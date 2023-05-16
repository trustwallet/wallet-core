use heck::ToLowerCamelCase;

use super::{AndroidMainMethod, AndroidMainParam, AndroidMainReturn, KotlinType};
use crate::codegen::swift::ObjectVariant;
use crate::manifest::FunctionInfo;
use crate::Result;

pub(super) fn process_android_main_methods(
    object: &ObjectVariant,
    functions: Vec<FunctionInfo>,
) -> Result<(Vec<AndroidMainMethod>, Vec<FunctionInfo>)> {
    let mut andmai_funcs = vec![];
    let mut skipped_funcs = vec![];

    for func in functions {
        if !func.name.starts_with(object.name()) {
            skipped_funcs.push(func);
            continue;
        }

        let params = func
            .params
            .into_iter()
            .map(|i| AndroidMainParam {
                name: i.name,
                ty: KotlinType::from(i.ty.variant),
                is_nullable: i.ty.is_nullable,
            })
            .collect();

        let pretty_name = func
            .name
            .strip_prefix(object.name())
            .unwrap()
            .to_lower_camel_case();

        andmai_funcs.push(AndroidMainMethod {
            name: pretty_name,
            params,
            is_static: func.is_static,
            return_ty: AndroidMainReturn {
                ty: KotlinType::from(func.return_type.variant),
                is_nullable: func.return_type.is_nullable,
            },
        });
    }

    Ok((andmai_funcs, skipped_funcs))
}
