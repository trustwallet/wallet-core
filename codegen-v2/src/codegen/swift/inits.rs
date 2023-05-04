use super::*;
use crate::manifest::InitInfo;

/// This function checks each constructor and determines whether there's an
/// association with the passed on object (struct or enum), based on common name
/// prefix, and maps the data into a Swift structure.
///
/// This function returns a tuple of associated Swift constructor and the skipped
/// respectively non-associated constructors.
pub(super) fn process_inits(
    object: &ObjectVariant,
    inits: Vec<InitInfo>,
) -> Result<(Vec<SwiftInit>, Vec<InitInfo>)> {
    let mut swift_inits = vec![];
    let mut skipped_inits = vec![];

    for init in inits {
        if !init.name.starts_with(object.name()) {
            // Init is not assciated with the object.
            skipped_inits.push(init);
            continue;
        }

        let mut ops = vec![];

        // For each parameter, we track a list of `params` which is used for the
        // function interface and add the necessary operations on how to process
        // those parameters.
        let mut params = vec![];
        for param in init.params {
            // Convert parameter to Swift parameter.
            params.push(SwiftParam {
                name: param.name.clone(),
                param_type: SwiftType::try_from(param.ty.variant.clone()).unwrap(),
                is_nullable: param.ty.is_nullable,
            });

            if let Some(op) = param_c_ffi_call(&param) {
                ops.push(op);
            }
        }

        // Prepepare parameter list to be passed on to the underlying C FFI function.
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

        // Prettify name, remove object name prefix from this property.
        let pretty_name = init
            .name
            .strip_prefix(object.name())
            // Panicing implies bug, checked at the start of the loop.
            .unwrap()
            .to_string();

        swift_inits.push(SwiftInit {
            name: pretty_name,
            is_nullable: init.is_nullable,
            params,
            operations: ops,
            comments: vec![],
        });
    }

    Ok((swift_inits, skipped_inits))
}
