use super::*;
use crate::manifest::{FunctionInfo, TypeVariant};
use heck::ToLowerCamelCase;

pub(super) fn process_object_methods(
    object: &ObjectVariant,
    functions: Vec<FunctionInfo>,
) -> Result<(Vec<SwiftFunction>, Vec<FunctionInfo>)> {
    let mut swift_funcs = vec![];
    let mut info_funcs = vec![];

    for func in functions {
        if !func.name.starts_with(object.name()) {
            // Function is not assciated with the object.
            info_funcs.push(func);
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

        // Convert parameters.
        let mut params = vec![];
        for param in func.params {
            // Skip self parameter
            match &param.ty.variant {
                TypeVariant::Enum(name) | TypeVariant::Struct(name) if name == object.name() => {
                    continue
                }
                _ => {}
            }

            // Convert parameter to Swift parameter.
            params.push(SwiftParam {
                name: param.name.clone(),
                param_type: SwiftType::try_from(param.ty.variant.clone()).unwrap(),
                is_nullable: param.ty.is_nullable,
            });

            ops.push(match &param.ty.variant {
                TypeVariant::String => {
                    let (var_name, call, defer) = (
                        param.name.clone(),
                        format!("TWStringCreateWithNSString({})", param.name),
                        Some(format!("TWStringDelete({})", param.name)),
                    );

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
                TypeVariant::Struct(_) => {
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
                TypeVariant::Enum(enm) => {
                    let (var_name, call, defer) = (
                        param.name.clone(),
                        format!("{enm}(rawValue: {}.rawValue)", param.name),
                        None,
                    );

                    SwiftOperation::Call {
                        var_name,
                        call,
                        defer,
                    }
                }
                // Reference the parameter by name directly, as defined in the
                // function interface.
                _ => continue,
            });
        }

        // Call the underlying C FFI function, passing on the `obj` instance.
        //
        // E.g: `let result = TWSomeFunc(obj)`.
        let param_name = if func.is_static { vec![] } else { vec!["obj"] };
        let param_names = param_name
            .into_iter()
            .chain(params.iter().map(|p| p.name.as_str()))
            .collect::<Vec<&str>>()
            .join(",");

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
        //
        // E.g:
        // - `return TWStringNSString(result)`
        // - `return SomeEnum(rawValue: result.rawValue)`
        // - `return SomeStruct(rawValue: result)`
        ops.push(match &func.return_type.variant {
            TypeVariant::String => SwiftOperation::Return {
                call: "TWStringNSString(result)".to_string(),
            },
            TypeVariant::Data => SwiftOperation::Return {
                call: "TWDataNSData(result)".to_string(),
            },
            TypeVariant::Enum(_enm) => SwiftOperation::Return {
                call: format!(
                    "{}(rawValue: result.rawValue)",
                    // TODO: Comment
                    // TODO: impl Display for SwiftType
                    SwiftType::try_from(func.return_type.variant.clone()).unwrap()
                ),
            },
            TypeVariant::Struct(_strct) => SwiftOperation::Return {
                call: format!(
                    "{}(rawValue: result)",
                    SwiftType::try_from(func.return_type.variant.clone()).unwrap()
                ),
            },
            _ => SwiftOperation::Return {
                call: "result".to_string(),
            },
        });

        // Convert return type.
        let return_type = SwiftReturn {
            param_type: SwiftType::try_from(func.return_type.variant).unwrap(),
            is_nullable: func.return_type.is_nullable,
        };

        let mut func_name = func
            .name
            .strip_prefix(object.name())
            .unwrap()
            .to_lower_camel_case();

        // Some functions do not follow standard camelCase convention.
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

    Ok((swift_funcs, info_funcs))
}
