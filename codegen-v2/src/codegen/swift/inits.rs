use super::*;
use crate::manifest::{InitInfo, TypeVariant};

pub(super) fn process_inits(
    object: &ObjectVariant,
    inits: Vec<InitInfo>,
) -> Result<(Vec<SwiftInit>, Vec<InitInfo>)> {
    let mut swift_inits = vec![];
    let mut info_inits = vec![];

    for init in inits {
        // TODO: The current/old codgen simply skips non-exported methods. Maybe
        // this should be renamed to `export` rather than `is_public`.
        if !init.name.starts_with(object.name()) || !init.is_public {
            // Init is not assciated with the object.
            info_inits.push(init);
            continue;
        }

        let mut ops = vec![];

        let mut params = vec![];
        for param in init.params {
            // Convert parameter to Swift parameter.
            params.push(SwiftParam {
                name: param.name.clone(),
                param_type: SwiftType::try_from(param.ty.variant.clone()).unwrap(),
                is_nullable: param.ty.is_nullable,
            });

            let (var_name, call, defer) = match &param.ty.variant {
                TypeVariant::String => (
                    param.name.clone(),
                    format!("TWStringCreateWithNSString({})", param.name),
                    Some(format!("TWStringDelete({})", param.name)),
                ),
                TypeVariant::Data => (
                    param.name.clone(),
                    format!("TWDataCreateWithNSData({})", param.name),
                    Some(format!("TWDataDelete({})", param.name)),
                ),
                TypeVariant::Struct(_) => {
                    (param.name.clone(), format!("{}.rawValue", param.name), None)
                }
                TypeVariant::Enum(enm) => (
                    param.name.clone(),
                    format!("{enm}(rawValue: {}.rawValue)", param.name),
                    None,
                ),
                // Reference the parameter by name directly, as defined in the
                // function interface.
                _ => continue,
            };

            if param.ty.is_nullable {
                ops.push(SwiftOperation::CallOptional {
                    var_name,
                    call,
                    defer,
                })
            } else {
                ops.push(SwiftOperation::Call {
                    var_name,
                    call,
                    defer,
                })
            }
        }

        // Call the underlying C FFI function, passing on the `obj` instance.
        //
        // E.g: `let result = TWSomeFunc(obj)`.
        let param_names = params
            .iter()
            .map(|p| p.name.as_str())
            .collect::<Vec<&str>>()
            .join(",");

        if init.is_nullable {
            ops.push(SwiftOperation::GuardedCall {
                var_name: "result".to_string(),
                call: format!("{}({})", init.name, param_names),
            });
        } else {
            ops.push(SwiftOperation::Call {
                var_name: "result".to_string(),
                call: format!("{}({})", init.name, param_names),
                defer: None,
            });
        }

        // TODO: Expand comment
        //
        // E.g:
        // - `return TWStringNSString(result)`
        // - `return SomeEnum(rawValue: result.rawValue)`
        // - `return SomeStruct(rawValue: result)`
        // TODO: Should there be a SwiftOperation for Init?
        /*
        ops.push(match &object {
            ObjectVariant::Struct(strct) => SwiftOperation::Return {
                call: format!("{}(rawValue: result)", strct),
            },
            ObjectVariant::Enum(enm) => SwiftOperation::Return {
                call: format!("{}(rawValue: result.rawValue)", enm),
            },
        });
         */

        let pretty_name = init.name.strip_prefix(object.name()).unwrap().to_string();

        swift_inits.push(SwiftInit {
            name: pretty_name,
            is_nullable: init.is_nullable,
            params,
            operations: ops,
            comments: vec![],
        });
    }

    Ok((swift_inits, info_inits))
}