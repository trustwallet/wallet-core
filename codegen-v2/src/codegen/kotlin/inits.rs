use crate::manifest::InitInfo;
use crate::Result;
// TODO: Move this to `crate::codegen`.
use super::{AndroidMainInit, AndroidMainParam, KotlinType};
use crate::codegen::swift::ObjectVariant;
use heck::ToLowerCamelCase;

pub(super) fn process_android_main_inits(
    object: &ObjectVariant,
    inits: Vec<InitInfo>,
) -> Result<(Vec<AndroidMainInit>, Vec<InitInfo>)> {
    let mut andmain_inits = vec![];
    let mut skipped_inits = vec![];

    for init in inits {
        if !init.name.starts_with(object.name()) {
            skipped_inits.push(init);
            continue;
        }

        let mut params = Vec::with_capacity(init.params.len());
        for param in init.params {
            params.push(AndroidMainParam {
                name: param.name,
                ty: KotlinType::from(param.ty.variant),
                is_nullable: param.ty.is_nullable,
            });
        }

        // Prepepare parameter list to be passed on to the call function.
        let param_names = params
            .iter()
            .map(|p| p.name.as_str())
            .collect::<Vec<&str>>()
            .join(",");

        let pretty_name = init
            .name
            .strip_prefix(object.name())
            // Panicing implies bug, checked at the start of the loop.
            .unwrap()
            .to_lower_camel_case();

        let return_call = format!("{pretty_name}({param_names})");

        andmain_inits.push(AndroidMainInit {
            name: pretty_name,
            params,
            is_nullable: init.is_nullable,
            // Creates a constructor return value such as:
            // `... : this(createWithData(data, string))`
            return_call,
        });
    }

    Ok((andmain_inits, skipped_inits))
}
