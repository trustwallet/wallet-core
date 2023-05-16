use super::AndroidMainStruct;
use crate::codegen::kotlin::functions::process_android_main_methods;
use crate::codegen::kotlin::inits::process_android_main_inits;
use crate::codegen::kotlin::properties::process_android_main_properties;
use crate::codegen::swift::ObjectVariant;
use crate::manifest::FileInfo;
use crate::Result;
use handlebars::Handlebars;

pub fn pretty_name(name: String) -> String {
    name.replace("_", "").replace("TW", "").replace("Proto", "")
}

/// Convenience wrapper for setting copyright year when generating bindings.
// TODO: Unify with Swift.
#[derive(Debug, Clone, Serialize)]
pub struct WithYear<'a, T> {
    pub current_year: u64,
    #[serde(flatten)]
    pub data: &'a T,
}

pub struct RenderIntput<'a> {
    pub file_info: FileInfo,
    pub android_main_template: &'a str,
}

#[derive(Debug, Clone, Default)]
pub struct GeneratedAndroidMainTypes {
    pub structs: Vec<AndroidMainStruct>,
}

#[derive(Debug, Clone, Default)]
pub struct GeneratedAndroidMainTypesStrings {
    pub structs: Vec<(String, String)>,
}

pub fn render_to_strings<'a>(input: RenderIntput<'a>) -> Result<GeneratedAndroidMainTypesStrings> {
    // The current year for the copyright header in the generated bindings.
    let current_year = crate::current_year();
    // Convert the name into an appropriate format.
    let pretty_file_name = pretty_name(input.file_info.name.clone());

    let mut engine = Handlebars::new();
    engine.set_strict_mode(true);

    engine.register_partial("androidmain_struct", input.android_main_template)?;

    let generated = generate_android_main_types(input.file_info)?;
    let mut out_strings = GeneratedAndroidMainTypesStrings::default();

    for strct in generated.structs {
        let out = engine.render(
            "androidmain_struct",
            &WithYear {
                current_year,
                data: &strct,
            },
        )?;

        out_strings.structs.push((strct.struct_name, out));
    }

    Ok(out_strings)
}

pub fn generate_android_main_types(mut info: FileInfo) -> Result<GeneratedAndroidMainTypes> {
    let mut outputs = GeneratedAndroidMainTypes::default();

    for strct in info.structs {
        let obj = ObjectVariant::Struct(&strct.name);

        let (inits, methods, properties);
        (inits, info.inits) = process_android_main_inits(&obj, info.inits)?;
        (methods, info.functions) = process_android_main_methods(&obj, info.functions)?;
        (properties, info.properties) = process_android_main_properties(&obj, info.properties)?;

        // Avoid rendering empty structs.
        if inits.is_empty() && methods.is_empty() && properties.is_empty() {
            continue;
        }

        // Split methods into static and non-static methods.
        let static_methods = methods.iter().cloned().filter(|m| m.is_static).collect();
        let methods = methods.into_iter().filter(|m| !m.is_static).collect();

        // Convert the name into an appropriate format.
        let pretty_struct_name = pretty_name(strct.name.clone());

        outputs.structs.push(AndroidMainStruct {
            struct_name: pretty_struct_name,
            is_class: strct.is_class,
            inits,
            methods,
            static_methods,
            properties,
        });
    }

    Ok(outputs)
}
