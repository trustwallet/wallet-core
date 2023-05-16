use crate::manifest::PropertyInfo;
use crate::Result;
// TODO: Move this to `crate::codegen`.
use super::{AndroidMainProperty, AndroidMainReturn, KotlinType};
use crate::codegen::swift::ObjectVariant;
use heck::ToLowerCamelCase;

pub(super) fn process_android_main_properties(
    object: &ObjectVariant,
    properties: Vec<PropertyInfo>,
) -> Result<(Vec<AndroidMainProperty>, Vec<PropertyInfo>)> {
    let mut andman_props = vec![];
    let mut skipped_props = vec![];

    for prop in properties {
        if !prop.name.starts_with(object.name()) {
            skipped_props.push(prop);
            continue;
        }

        let pretty_name = prop
            .name
            .strip_prefix(object.name())
            .unwrap()
            .to_lower_camel_case();

        andman_props.push(AndroidMainProperty {
            name: pretty_name,
            return_ty: AndroidMainReturn {
                ty: KotlinType::from(prop.return_type.variant),
                is_nullable: prop.return_type.is_nullable,
            },
        });
    }

    Ok((andman_props, skipped_props))
}
