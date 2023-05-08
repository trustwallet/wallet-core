use super::*;

#[derive(Debug, Clone)]
pub struct RenderIntput<'a> {
    pub file_info: FileInfo,
    pub struct_template: &'a str,
    pub enum_template: &'a str,
    pub extension_template: &'a str,
    pub proto_template: &'a str,
}

#[derive(Debug, Clone, Default)]
pub struct RenderOutputStrings {
    pub structs: Vec<(String, String)>,
    pub enums: Vec<(String, String)>,
    pub extensions: Vec<(String, String)>,
    pub protos: Vec<(String, String)>,
}

#[derive(Debug, Clone, Default)]
pub struct RenderOutput {
    pub structs: Vec<SwiftStruct>,
    pub enums: Vec<SwiftEnum>,
    pub extensions: Vec<SwiftEnumExtension>,
    pub protos: Vec<SwiftProto>,
}

#[derive(Debug, Clone, Serialize)]
pub struct WithYear<'a, T> {
    pub current_year: u64,
    #[serde(flatten)]
    pub data: &'a T,
}

pub fn pretty_name(name: String) -> String {
    name.replace("_", "").replace("TW", "").replace("Proto", "")
}

pub fn render_file_info_strings<'a>(input: RenderIntput<'a>) -> Result<RenderOutputStrings> {
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

    let rendered = render_file_info(input)?;
    let mut out_str = RenderOutputStrings::default();

    //  Render structs.
    for strct in rendered.structs {
        let out = engine.render(
            "struct",
            &WithYear {
                current_year,
                data: &strct,
            },
        )?;

        out_str.structs.push((strct.name.to_string(), out));
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

        out_str.enums.push((enm.name.to_string(), out));
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

        out_str.extensions.push((ext.name.to_string(), out));
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
pub fn render_file_info<'a>(input: RenderIntput<'a>) -> Result<RenderOutput> {
    let mut info = input.file_info;
    let mut outputs = RenderOutput::default();

    // Render structs/classes.
    for strct in info.structs {
        let obj = ObjectVariant::Struct(&strct.name);

        // Process items.
        let (inits, mut methods, properties);
        (inits, info.inits) = process_inits(&obj, info.inits)?;
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
            init_instance: strct.is_class,
            superclasses,
            eq_operator,
            inits: inits,
            // TODO:
            deinits: info.deinits.clone(),
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
