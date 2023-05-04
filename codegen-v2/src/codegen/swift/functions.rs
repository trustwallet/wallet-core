use super::*;
use crate::manifest::{FunctionInfo, TypeVariant};
use heck::ToLowerCamelCase;

/// This function checks each function and determines whether there's an
/// association with the passed on object (struct or enum), based on common name
/// prefix, and maps the data into a Swift structure.
///
/// This function returns a tuple of associated Swift functions and the skipped
/// respectively non-associated functions.
pub(super) fn process_object_methods(
    object: &ObjectVariant,
    functions: Vec<FunctionInfo>,
) -> Result<(Vec<SwiftFunction>, Vec<FunctionInfo>)> {
    let mut swift_funcs = vec![];
    let mut skipped_funcs = vec![];

    for func in functions {
        if !func.name.starts_with(object.name()) {
            // Function is not assciated with the object.
            skipped_funcs.push(func);
            continue;
        }

        let mut ops = vec![];

        // Initalize the 'self' type, which is then passed on to the underlying
        // C FFI function, assuming the function is not static.
        //
        // E.g:
        // - `let obj = self.rawValue`
        // - `let obj = TWSomeEnum(rawValue: self.RawValue")`
        if !func.is_static {
            ops.push(match object {
                ObjectVariant::Struct(_) => SwiftOperation::Call {
                    var_name: "obj".to_string(),
                    call: "self.rawValue".to_string(),
                    defer: None,
                },
                ObjectVariant::Enum(name) => SwiftOperation::Call {
                    var_name: "obj".to_string(),
                    call: format!("{}(rawValue: self.rawValue)", name),
                    defer: None,
                },
            });
        }

        // For each parameter, we track a list of `params` which is used for the
        // function interface and add the necessary operations on how to process
        // those parameters.
        let mut params = vec![];
        for param in func.params {
            // Skip self parameter
            match &param.ty.variant {
                TypeVariant::Enum(name) | TypeVariant::Struct(name) if name == object.name() => {
                    continue
                }
                _ => {}
            }

            // Convert parameter to Swift parameter for the function interface.
            params.push(SwiftParam {
                name: param.name.clone(),
                param_type: SwiftType::from(param.ty.variant.clone()),
                is_nullable: param.ty.is_nullable,
            });

            // Process parameter.
            ops.push(match &param.ty.variant {
                // E.g. `let param = TWStringCreateWithNSString(param)`
                TypeVariant::String => {
                    let (var_name, call, defer) = (
                        param.name.clone(),
                        format!("TWStringCreateWithNSString({})", param.name),
                        Some(format!("TWStringDelete({})", param.name)),
                    );

                    // If the parameter is nullable, add special handler.
                    if param.ty.is_nullable {
                        SwiftOperation::CallOptional {
                            var_name,
                            call,
                            defer,
                        }
                    } else {
                        SwiftOperation::Call {
                            var_name,
                            call,
                            defer,
                        }
                    }
                }
                TypeVariant::Data => {
                    let (var_name, call, defer) = (
                        param.name.clone(),
                        format!("TWDataCreateWithNSData({})", param.name),
                        Some(format!("TWDataDelete({})", param.name)),
                    );

                    // If the parameter is nullable, add special handler.
                    if param.ty.is_nullable {
                        SwiftOperation::CallOptional {
                            var_name,
                            call,
                            defer,
                        }
                    } else {
                        SwiftOperation::Call {
                            var_name,
                            call,
                            defer,
                        }
                    }
                }
                // E.g.
                // - `let param = param.rawValue`
                // - `let param = param?.rawValue`
                TypeVariant::Struct(_) => {
                    // For nullable structs, we do not use the special
                    // `CallOptional` handler but rather use the question mark
                    // operator.
                    let (var_name, call, defer) = if param.ty.is_nullable {
                        (
                            param.name.clone(),
                            format!("{}?.rawValue", param.name),
                            None,
                        )
                    } else {
                        (param.name.clone(), format!("{}.rawValue", param.name), None)
                    };

                    SwiftOperation::Call {
                        var_name,
                        call,
                        defer,
                    }
                }
                // E.g. `let param = TWSomeEnum(rawValue: param.rawValue)`
                // Note that it calls the constructor of the enum, which calls
                // the underlying "*Create*" C FFI function.
                TypeVariant::Enum(enm) => {
                    SwiftOperation::Call {
                        var_name: param.name.clone(),
                        call: format!("{enm}(rawValue: {}.rawValue)", param.name),
                        defer: None,
                    }
                }
                // Skip processing parameter, reference the parameter by name
                // directly, as defined in the function interface (usually the
                // case for primitive types).
                _ => continue,
            });
        }

        // Prepepare parameter list to be passed on to the underlying C FFI function.
        let param_name = if func.is_static { vec![] } else { vec!["obj"] };
        let param_names = param_name
            .into_iter()
            .chain(params.iter().map(|p| p.name.as_str()))
            .collect::<Vec<&str>>()
            .join(",");

        // Call the underlying C FFI function, passing on the parameter list.
        if func.return_type.is_nullable {
            ops.push(SwiftOperation::GuardedCall {
                var_name: "result".to_string(),
                call: format!("{}({})", func.name, param_names),
            });
        } else {
            ops.push(SwiftOperation::Call {
                var_name: "result".to_string(),
                call: format!("{}({})", func.name, param_names),
                defer: None,
            });
        }

        // The `result` must be handled and returned explicitly.
        ops.push(match &func.return_type.variant {
            // E.g. `return TWStringNSString(result)`
            TypeVariant::String => SwiftOperation::Return {
                call: "TWStringNSString(result)".to_string(),
            },
            TypeVariant::Data => SwiftOperation::Return {
                call: "TWDataNSData(result)".to_string(),
            },
            // E.g. `return SomeEnum(rawValue: result.rawValue)`
            TypeVariant::Enum(_enm) => SwiftOperation::Return {
                call: format!(
                    "{}(rawValue: result.rawValue)",
                    // Strip "TW" prefix from enum, impying Swift enum.
                    SwiftType::from(func.return_type.variant.clone())
                ),
            },
            // E.g. `return SomeStruct(rawValue: result)`
            TypeVariant::Struct(_strct) => SwiftOperation::Return {
                call: format!(
                    "{}(rawValue: result)",
                    // Strip "TW" prefix from struct, impying Swift struct.
                    SwiftType::from(func.return_type.variant.clone())
                ),
            },
            // E.g. `return result`
            _ => SwiftOperation::Return {
                call: "result".to_string(),
            },
        });

        // Convert return type.
        let return_type = SwiftReturn {
            param_type: SwiftType::from(func.return_type.variant),
            is_nullable: func.return_type.is_nullable,
        };

        let mut func_name = func
            .name
            .strip_prefix(object.name())
            // Panicing implies bug, checked at the start of the loop.
            .unwrap()
            .to_lower_camel_case();

        // Special handling: some functions do not follow standard camelCase
        // convention.
        if object.name() == "TWStoredKey" {
            func_name = func_name.replace("Json", "JSON");
            func_name = func_name.replace("Hd", "HD");
        } else if object.name() == "TWPublicKey" {
            func_name = func_name.replace("Der", "DER");
        } else if object.name() == "TWHash" {
            func_name = func_name.replace("ripemd", "RIPEMD");
            func_name = func_name.replace("Ripemd", "RIPEMD");
            func_name = func_name.replace("sha512256", "sha512_256");
            func_name = func_name.replace("sha3256", "sha3_256");
            func_name = func_name.replace("sha256sha256", "sha256SHA256");
        } else if object.name() == "TWAES" {
            func_name = func_name.replace("Cbc", "CBC");
            func_name = func_name.replace("Ctr", "CTR");
        }

        swift_funcs.push(SwiftFunction {
            name: func_name,
            is_public: func.is_public,
            is_static: func.is_static,
            operations: ops,
            params,
            return_type,
            comments: vec![],
        });
    }

    Ok((swift_funcs, skipped_funcs))
}
