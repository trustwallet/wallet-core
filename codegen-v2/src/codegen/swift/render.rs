// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use super::{inits::process_deinits, *};

#[derive(Debug, Clone)]
pub struct RenderIntput<'a> {
    pub file_info: FileInfo,
    pub struct_template: &'a str,
    pub enum_template: &'a str,
    pub extension_template: &'a str,
    pub proto_template: &'a str,
    pub partial_init_template: &'a str,
    pub partial_func_tempalte: &'a str,
    pub partial_prop_tempalte: &'a str,
}

#[derive(Debug, Clone, Default)]
pub struct GeneratedSwiftTypesStrings {
    pub structs: Vec<(String, String)>,
    pub enums: Vec<(String, String)>,
    pub extensions: Vec<(String, String)>,
    pub protos: Vec<(String, String)>,
}

#[derive(Debug, Clone, Default)]
pub struct GeneratedSwiftTypes {
    pub structs: Vec<SwiftStruct>,
    pub enums: Vec<SwiftEnum>,
    pub extensions: Vec<SwiftEnumExtension>,
    pub protos: Vec<SwiftProto>,
}

/// Convenience wrapper for setting copyright year when generating bindings.
#[derive(Debug, Clone, Serialize)]
struct WithYear<'a, T> {
    pub current_year: u64,
    #[serde(flatten)]
    pub data: &'a T,
}

pub fn pretty_name(name: String) -> String {
    name.replace("_", "").replace("TW", "").replace("Proto", "")
}

pub fn render_to_strings<'a>(input: RenderIntput<'a>) -> Result<GeneratedSwiftTypesStrings> {
    // The current year for the copyright header in the generated bindings.
    let current_year = crate::current_year();
    // Convert the name into an appropriate format.
    let pretty_file_name = pretty_name(input.file_info.name.clone());

    let mut engine = Handlebars::new();
    // Unmatched variables should result in an error.
    engine.set_strict_mode(true);

    engine.register_partial("struct", input.struct_template)?;
    engine.register_partial("enum", input.enum_template)?;
    engine.register_partial("extension", input.extension_template)?;
    engine.register_partial("proto", input.proto_template)?;
    engine.register_partial("partial_init", input.partial_init_template)?;
    engine.register_partial("partial_func", input.partial_func_tempalte)?;
    engine.register_partial("partial_prop", input.partial_prop_tempalte)?;

    let rendered = generate_swift_types(input.file_info)?;
    let mut out_str = GeneratedSwiftTypesStrings::default();

    //  Render structs.
    for strct in rendered.structs {
        let out = engine.render(
            "struct",
            &WithYear {
                current_year,
                data: &strct,
            },
        )?;

        out_str.structs.push((strct.name, out));
    }

    //  Render enums.
    for enm in rendered.enums {
        let out = engine.render(
            "enum",
            &WithYear {
                current_year,
                data: &enm,
            },
        )?;

        out_str.enums.push((enm.name, out));
    }

    //  Render extensions.
    for ext in rendered.extensions {
        let out = engine.render(
            "extension",
            &WithYear {
                current_year,
                data: &ext,
            },
        )?;

        out_str.extensions.push((ext.name, out));
    }

    //  Render protos.
    if !rendered.protos.is_empty() {
        let out = engine.render(
            "proto",
            &WithYear {
                current_year,
                data: &json!({
                    "protos": &rendered.protos
                }),
            },
        )?;

        out_str.protos.push((pretty_file_name, out));
    }

    Ok(out_str)
}

/// Uses the given input templates to render all files.
pub fn generate_swift_types(mut info: FileInfo) -> Result<GeneratedSwiftTypes> {
    let mut outputs = GeneratedSwiftTypes::default();

    // Render structs/classes.
    for strct in info.structs {
        let obj = ObjectVariant::Struct(&strct.name);

        // Process items.
        let (inits, deinits, mut methods, properties);
        (inits, info.inits) = process_inits(&obj, info.inits)?;
        (deinits, info.deinits) = process_deinits(&obj, info.deinits)?;
        (methods, info.functions) = process_methods(&obj, info.functions)?;
        (properties, info.properties) = process_properties(&obj, info.properties)?;

        // Avoid rendering empty structs.
        if inits.is_empty() && methods.is_empty() && properties.is_empty() {
            continue;
        }

        // Convert the name into an appropriate format.
        let pretty_struct_name = pretty_name(strct.name.clone());

        // Add superclasses.
        let superclasses = if pretty_struct_name.ends_with("Address") {
            vec!["Address".to_string()]
        } else {
            vec![]
        };

        // Handle equality operator.
        let eq_method = methods.iter().enumerate().find(|(_, f)| f.name == "equal");
        let eq_operator = if let Some((idx, _)) = eq_method {
            let operator = SwiftOperatorEquality {
                c_ffi_name: format!("{}Equal", strct.name),
            };

            // Remove that method from the `methods` list.
            methods.remove(idx);

            Some(operator)
        } else {
            None
        };

        outputs.structs.push(SwiftStruct {
            name: pretty_struct_name,
            is_class: strct.is_class,
            is_public: strct.is_public,
            init_instance: strct.is_class,
            superclasses,
            eq_operator,
            inits: inits,
            deinits: deinits,
            methods,
            properties,
        });
    }

    // Render enums.
    for enm in info.enums {
        let obj = ObjectVariant::Enum(&enm.name);

        // Process items.
        let (methods, properties);
        (methods, info.functions) = process_methods(&obj, info.functions)?;
        (properties, info.properties) = process_properties(&obj, info.properties)?;

        // Convert the name into an appropriate format.
        let pretty_enum_name = pretty_name(enm.name);

        // Add superclasses.
        let value_type = SwiftType::from(enm.value_type);
        let mut superclasses = vec![value_type.0, "CaseIterable".to_string()];

        let mut add_class = false;

        // Convert to Swift enum variants
        let variants = enm
            .variants
            .into_iter()
            .map(|info| {
                if info.as_string.is_some() {
                    add_class = true;
                }

                SwiftEnumVariant {
                    name: info.name,
                    value: info.value,
                    as_string: info.as_string,
                }
            })
            .collect();

        if add_class {
            superclasses.push("CustomStringConvertible".to_string());
        }

        outputs.enums.push(SwiftEnum {
            name: pretty_enum_name.clone(),
            is_public: enm.is_public,
            add_description: add_class,
            superclasses,
            variants,
        });

        // Avoid rendering empty extension for enums.
        if methods.is_empty() && properties.is_empty() {
            continue;
        }

        outputs.extensions.push(SwiftEnumExtension {
            name: pretty_enum_name,
            init_instance: true,
            methods,
            properties,
        });
    }

    // Render Protobufs.
    if !info.protos.is_empty() {
        for proto in info.protos {
            outputs.protos.push(SwiftProto::try_from(proto)?);
        }
    }

    Ok(outputs)
}
