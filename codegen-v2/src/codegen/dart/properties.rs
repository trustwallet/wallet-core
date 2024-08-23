// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::*;
use crate::manifest::PropertyInfo;
use heck::ToLowerCamelCase;

/// This function checks each property and determines whether there's an
/// association with the passed on object (struct or enum), based on common name
/// prefix, and maps the data into a Swift structure.
///
/// This function returns a tuple of associated Swift properties and skipped
/// respectively non-associated properties.
pub(super) fn process_properties(
    object: &ObjectVariant,
    properties: Vec<PropertyInfo>,
) -> Result<(Vec<DartProperty>, Vec<PropertyInfo>)> {
    let mut dart_props = vec![];
    let mut skipped_props = vec![];

    for prop in properties {
        if !prop.name.starts_with(object.name()) {
            // Property is not assciated with the object.
            skipped_props.push(prop);
            continue;
        }

        let mut ops = vec![];

        // Initialize the 'self' type, which is then passed on to the underlying
        // C FFI function.
        ops.push(match object {
            // E.g. `let obj = self.rawValue`
            ObjectVariant::Struct(_) => DartOperation::Call {
                var_name: "obj".to_string(),
                call: "self.rawValue".to_string(),
            },
            // E.g. `let obj = TWSomeEnum(rawValue: self.rawValue")`
            ObjectVariant::Enum(name) => DartOperation::Call {
                var_name: "obj".to_string(),
                call: format!("{}(rawValue: self.rawValue)", name),
            },
        });

        // Call the underlying C FFI function, passing on the `obj` instance.
        //
        // E.g: `final result = TWSomeFunc(obj);`.
        let (var_name, call) = ("result".to_string(), format!("{}(obj)", prop.name));
        if prop.return_type.is_nullable {
            ops.push(DartOperation::GuardedCall { var_name, call });
        } else {
            ops.push(DartOperation::Call {
                var_name,
                call,
            });
        }

        // Wrap result.
        ops.push(wrap_return(&prop.return_type));

        // Prettify name, remove object name prefix from this property.
        let pretty_name = prop
            .name
            .strip_prefix(object.name())
            // Panicking implies bug, checked at the start of the loop.
            .unwrap()
            .to_lower_camel_case();

        // Convert return type for property interface.
        let return_type = DartReturn {
            param_type: DartType::from(prop.return_type.variant),
            is_nullable: prop.return_type.is_nullable,
        };

        dart_props.push(DartProperty {
            name: pretty_name,
            is_public: prop.is_public,
            is_override: true,
            operations: ops,
            return_type,
            comments: vec![],
        });
    }

    Ok((dart_props, skipped_props))
}
