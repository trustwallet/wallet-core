use crate::manifest::{
    FileInfo, FunctionInfo, InitInfo, ParamInfo, PropertyInfo, ProtoInfo, TypeInfo, TypeVariant,
};
use crate::{Error, Result};
use handlebars::Handlebars;
use serde_json::json;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftType(String);

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftFunction {
    pub name: String,
    pub is_public: bool,
    pub is_static: bool,
    pub params: Vec<SwiftParam>,
    pub operations: Vec<SwiftOperation>,
    #[serde(rename = "return")]
    pub return_type: SwiftReturn,
    pub comments: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct SwiftProperty {
    pub name: String,
    pub is_public: bool,
    pub operations: Vec<SwiftOperation>,
    #[serde(rename = "return")]
    pub return_type: SwiftReturn,
    pub comments: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
#[serde(rename_all = "snake_case")]
pub enum SwiftOperation {
    Call {
        var_name: String,
        call: String,
        defer: Option<String>,
    },
    GuardedCall {
        var_name: String,
        call: String,
    },
    Return {
        call: String,
    },
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftParam {
    pub name: String,
    #[serde(rename = "type")]
    pub param_type: SwiftType,
    pub is_nullable: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftReturn {
    #[serde(rename = "type")]
    pub param_type: SwiftType,
    pub is_nullable: bool,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftInit {
    pub name: String,
    pub is_nullable: bool,
    pub params: Vec<SwiftParam>,
    pub operations: Vec<SwiftOperation>,
    pub comments: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftProto {
    pub name: String,
    pub c_ffi_name: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftOperatorEquality {
    pub c_ffi_name: String,
    pub is_public: bool,
    pub is_static: bool,
}

#[derive(Debug, Clone)]
pub struct RenderIntput<'a> {
    pub file_info: FileInfo,
    pub struct_template: &'a str,
    pub enum_template: &'a str,
    pub extension_template: &'a str,
    pub proto_template: &'a str,
}

#[derive(Debug, Clone, Default)]
pub struct RenderOutput {
    pub structs: Vec<(String, String)>,
    pub enums: Vec<(String, String)>,
    pub extensions: Vec<(String, String)>,
    pub protos: Vec<(String, String)>,
}

pub fn render_file_info<'a>(input: RenderIntput<'a>) -> Result<RenderOutput> {
    let mut engine = Handlebars::new();
    // Unmatched variables should result in an error.
    engine.set_strict_mode(true);

    engine
        .register_partial("struct", input.struct_template)
        .unwrap();
    engine
        .register_partial("enum", input.enum_template)
        .unwrap();
    engine
        .register_partial("extension", input.extension_template)
        .unwrap();
    engine
        .register_partial("proto", input.proto_template)
        .unwrap();

    let mut info = input.file_info;
    let mut outputs = RenderOutput::default();

    // Render structs/classes.
    for strct in info.structs {
        let is_class = strct.tags.iter().any(|t| t == "TW_EXPORT_CLASS");

        let (inits, mut methods, properties);
        (inits, info.inits) =
            process_inits(&ObjectVariant::Struct(&strct.name), info.inits).unwrap();
        (methods, info.functions) =
            process_object_methods(&ObjectVariant::Struct(&strct.name), info.functions).unwrap();
        (properties, info.properties) =
            process_object_properties(&ObjectVariant::Struct(&strct.name), info.properties)
                .unwrap();

        // Avoid rendering empty structs.
        if inits.is_empty() && methods.is_empty() && properties.is_empty() {
            continue;
        }

        let struct_name = strct.name.strip_prefix("TW").ok_or(Error::Todo)?;

        // Add superclasses.
        let superclasses = if struct_name.ends_with("Address") {
            vec!["Address"]
        } else {
            vec![]
        };

        // Handle equality operator.
        let equality_method = methods.iter().enumerate().find(|(_, f)| f.name == "equal");

        let equality_operator = if let Some((idx, func)) = equality_method {
            let operator = SwiftOperatorEquality {
                c_ffi_name: func.name.clone(),
                is_public: func.is_public,
                is_static: func.is_static,
            };

            // Remove that method from the `methods` list.
            methods.remove(idx);

            Some(operator)
        } else {
            None
        };

        let payload = json!({
            "name": struct_name,
            "is_class": is_class,
            "init_instance": is_class,
            "superclasses": superclasses,
            "eq_operator": equality_operator,
            "inits": inits,
            "deinits": info.deinits,
            "methods": methods,
            "properties": properties,
        });

        // TODO
        //println!("{}", serde_json::to_string_pretty(&payload).unwrap());

        let out = engine.render("struct", &payload).unwrap();

        outputs.structs.push((struct_name.to_string(), out));
    }

    // Render enums.
    for enm in info.enums {
        let (methods, properties);
        (methods, info.functions) =
            process_object_methods(&ObjectVariant::Enum(&enm.name), info.functions).unwrap();
        (properties, info.properties) =
            process_object_properties(&ObjectVariant::Enum(&enm.name), info.properties).unwrap();

        let enum_name = enm.name.strip_prefix("TW").ok_or(Error::Todo)?;

        // Add superclasses.
        let value_type = SwiftType::from(enm.value_type);
        let mut superclasses = vec![value_type.0.as_str(), "CaseIterable"];

        // If the enum has `as_string` fields, we can generate a description.
        let description: Option<Vec<(&str, &str)>> =
            if enm.variants.iter().any(|e| e.as_string.is_some()) {
                superclasses.push("CustomStringConvertible");

                Some(
                    enm.variants
                        .iter()
                        // TODO: Unwrap must be handled:
                        .map(|e| (e.name.as_str(), e.as_string.as_ref().unwrap().as_str()))
                        .collect(),
                )
            } else {
                None
            };

        let enum_payload = json!({
            "name": enum_name,
            "is_public": enm.is_public,
            "superclasses": superclasses,
            "variants": enm.variants,
            "description": description,
        });

        let out = engine.render("enum", &enum_payload).unwrap();
        outputs.enums.push((enum_name.to_string(), out));

        // Avoid rendering empty extension for enums.
        if methods.is_empty() && properties.is_empty() {
            continue;
        }

        let extension_payload = json!({
            "name": enum_name,
            "init_instance": true,
            "parent_classes": [],
            "methods": methods,
            "properties": properties,
        });

        let out = engine.render("extension", &extension_payload).unwrap();
        outputs.extensions.push((enum_name.to_string(), out));
    }

    // Render Protobufs.
    if !info.protos.is_empty() {
        // TODO: Should this convention be enforced?
        let file_name = info
            .name
            .strip_prefix("TW")
            .ok_or(Error::Todo)?
            .strip_suffix("Proto")
            .ok_or(Error::Todo)?
            .to_string();

        let protos = info
            .protos
            .into_iter()
            .map(SwiftProto::try_from)
            .collect::<Result<Vec<_>>>()?;

        let payload = json!({
            "protos": protos,
        });

        let out = engine.render("proto", &payload).unwrap();
        outputs.protos.push((file_name, out));
    }

    Ok(outputs)
}

fn process_inits(
    object: &ObjectVariant,
    inits: Vec<InitInfo>,
) -> Result<(Vec<SwiftInit>, Vec<InitInfo>)> {
    let mut swift_inits = vec![];
    let mut info_inits = vec![];

    for init in inits {
        if !init.name.starts_with(object.name()) {
            // Init is not assciated with the object.
            info_inits.push(init);
            continue;
        }

        let mut ops = vec![];

        let mut params = vec![];
        for param in init.params {
            let call = match &param.ty.variant {
                TypeVariant::String => Some(SwiftOperation::Call {
                    var_name: param.name.clone(),
                    call: format!("TWStringCreateWithNSString({})", param.name),
                    defer: Some(format!("TWStringDelete({})", param.name)),
                }),
                TypeVariant::Data => Some(SwiftOperation::Call {
                    var_name: param.name.clone(),
                    call: format!("TWDataCreateWithNSData({})", param.name),
                    defer: Some(format!("TWDataDelete({})", param.name)),
                }),
                TypeVariant::Enum(enm) => Some(SwiftOperation::Call {
                    var_name: param.name.clone(),
                    call: format!("{enm}(rawValue: {}.rawValue)", param.name),
                    defer: None,
                }),
                // Reference the parameter by name directly, as defined in the
                // function interface.
                _ => None,
            };

            if let Some(call) = call {
                ops.push(call);
            }

            // Convert parameter to Swift parameter.
            params.push(SwiftParam {
                name: param.name,
                param_type: SwiftType::try_from(param.ty.variant).unwrap(),
                is_nullable: param.ty.is_nullable,
            });
        }

        // Call the underlying C FFI function, passing on the `obj` instance.
        //
        // E.g: `let result = TWSomeFunc(obj)`.
        let param_names = params
            .iter()
            .map(|p| p.name.as_str())
            .collect::<Vec<&str>>()
            .join(",");

        ops.push(SwiftOperation::Call {
            var_name: "result".to_string(),
            call: format!("{}({})", init.name, param_names),
            defer: None,
        });

        // The `result` must be handled and returned explicitly.
        //
        // E.g:
        // - `return TWStringNSString(result)`
        // - `return SomeEnum(rawValue: result.rawValue)`
        // - `return SomeStruct(rawValue: result)`
        ops.push(match &object {
            ObjectVariant::Struct(strct) => SwiftOperation::Return {
                call: format!("{}(rawValue: result)", strct),
            },
            ObjectVariant::Enum(enm) => SwiftOperation::Return {
                call: format!("{}(rawValue: result.rawValue)", enm),
            },
        });

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

fn process_object_methods(
    object: &ObjectVariant,
    functions: Vec<FunctionInfo>,
) -> Result<(Vec<SwiftFunction>, Vec<FunctionInfo>)> {
    let mut swift_funcs = vec![];
    let mut info_funcs = vec![];

    for func in functions {
        // TODO: This should be handled by the manifest
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
            // TODO: This should be set stricter by the C header, respectively
            // the manifest.
            match &param.ty.variant {
                TypeVariant::Enum(name) | TypeVariant::Struct(name) if name == object.name() => {
                    continue
                }
                _ => {}
            }

            let call = match &param.ty.variant {
                TypeVariant::String => Some(SwiftOperation::Call {
                    var_name: param.name.clone(),
                    call: format!("TWStringCreateWithNSString({})", param.name),
                    defer: Some(format!("TWStringDelete({})", param.name)),
                }),
                TypeVariant::Data => Some(SwiftOperation::Call {
                    var_name: param.name.clone(),
                    call: format!("TWDataCreateWithNSData({})", param.name),
                    defer: Some(format!("TWDataDelete({})", param.name)),
                }),
                TypeVariant::Enum(enm) => Some(SwiftOperation::Call {
                    var_name: param.name.clone(),
                    call: format!("{enm}(rawValue: {}.rawValue)", param.name),
                    defer: None,
                }),
                // Reference the parameter by name directly, as defined in the
                // function interface.
                _ => None,
            };

            if let Some(call) = call {
                ops.push(call);
            }

            // Convert parameter to Swift parameter.
            params.push(SwiftParam {
                name: param.name,
                param_type: SwiftType::try_from(param.ty.variant).unwrap(),
                is_nullable: param.ty.is_nullable,
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

        ops.push(SwiftOperation::Call {
            var_name: "result".to_string(),
            call: format!("{}({})", func.name, param_names),
            defer: None,
        });

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
            TypeVariant::Enum(enm) => SwiftOperation::Return {
                call: format!("{}(rawValue: result.rawValue)", enm),
            },
            TypeVariant::Struct(strct) => SwiftOperation::Return {
                call: format!("{}(rawValue: result)", strct),
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

        let func_name = func.name.strip_prefix(object.name()).unwrap().to_string();

        swift_funcs.push(SwiftFunction {
            name: first_char_to_lowercase(func_name),
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

enum ObjectVariant<'a> {
    Struct(&'a str),
    Enum(&'a str),
}

impl<'a> ObjectVariant<'a> {
    fn name(&'a self) -> &'a str {
        match self {
            ObjectVariant::Struct(n) | ObjectVariant::Enum(n) => n,
        }
    }
}

fn process_object_properties(
    object: &ObjectVariant,
    properties: Vec<PropertyInfo>,
) -> Result<(Vec<SwiftProperty>, Vec<PropertyInfo>)> {
    let mut swift_props = vec![];
    let mut info_props = vec![];

    for prop in properties {
        // TODO: This should be handled by the manifest
        if !prop.name.starts_with(object.name()) {
            // Property is not assciated with the object.
            info_props.push(prop);
            continue;
        }

        let mut ops = vec![];

        // Initalize the 'self' type, which is then passed on to the underlying
        // C FFI function.
        //
        // E.g:
        // - `let obj = self.rawValue`
        // - `let obj = TWSomeEnum(rawValue: self.RawValue")`
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

        // Call the underlying C FFI function, passing on the `obj` instance.
        //
        // E.g: `let result = TWSomeFunc(obj)`.
        ops.push(SwiftOperation::Call {
            var_name: "result".to_string(),
            call: format!("{}(obj)", prop.name),
            defer: None,
        });

        // The `result` must be handled and returned explicitly.
        //
        // E.g:
        // - `return TWStringNSString(result)`
        // - `return SomeEnum(rawValue: result.rawValue)`
        // - `return SomeStruct(rawValue: result)`
        ops.push(match &prop.return_type.variant {
            TypeVariant::String => SwiftOperation::Return {
                call: "TWStringNSString(result)".to_string(),
            },
            TypeVariant::Data => SwiftOperation::Return {
                call: "TWDataNSData(result)".to_string(),
            },
            TypeVariant::Enum(enm) => SwiftOperation::Return {
                call: format!("{}(rawValue: result.rawValue)", enm),
            },
            TypeVariant::Struct(strct) => SwiftOperation::Return {
                call: format!("{}(rawValue: result)", strct),
            },
            _ => SwiftOperation::Return {
                call: "result".to_string(),
            },
        });

        // Pretty name.
        let mut pretty_name = prop.name.strip_prefix(object.name()).unwrap().to_string();

        // TODO/TEMP
        if &pretty_name == "HRP" {
            pretty_name = "hrp".to_string();
        }

        // Convert return type.
        let return_type = SwiftReturn {
            param_type: SwiftType::try_from(prop.return_type.variant).unwrap(),
            is_nullable: prop.return_type.is_nullable,
        };

        swift_props.push(SwiftProperty {
            name: first_char_to_lowercase(pretty_name),
            is_public: prop.is_public,
            operations: ops,
            return_type,
            comments: vec![],
        });
    }

    Ok((swift_props, info_props))
}

pub fn first_char_to_lowercase(input_str: String) -> String {
    let mut chars = input_str.chars();
    match chars.next() {
        None => String::new(),
        Some(f) => f.to_lowercase().chain(chars).collect(),
    }
}

impl From<TypeVariant> for SwiftType {
    fn from(value: TypeVariant) -> Self {
        let res = match value {
            TypeVariant::Void => "()".to_string(),
            TypeVariant::Bool => "Bool".to_string(),
            TypeVariant::Char => "Character".to_string(),
            TypeVariant::ShortInt => "Int16".to_string(),
            TypeVariant::Int => "Int32".to_string(),
            TypeVariant::UnsignedInt => "UInt32".to_string(),
            TypeVariant::LongInt => "Int64".to_string(),
            TypeVariant::Float => "Float".to_string(),
            TypeVariant::Double => "Double".to_string(),
            TypeVariant::SizeT => "Int".to_string(),
            TypeVariant::Int8T => "Int8".to_string(),
            TypeVariant::Int16T => "Int16".to_string(),
            TypeVariant::Int32T => "Int32".to_string(),
            TypeVariant::Int64T => "Int64".to_string(),
            TypeVariant::UInt8T => "UInt8".to_string(),
            TypeVariant::UInt16T => "UInt16".to_string(),
            TypeVariant::UInt32T => "UInt32".to_string(),
            TypeVariant::UInt64T => "UInt64".to_string(),
            TypeVariant::String => "String".to_string(),
            TypeVariant::Data => "Data".to_string(),
            TypeVariant::Struct(n) | TypeVariant::Enum(n) => {
                n.strip_prefix("TW").unwrap().to_string()
            }
        };

        SwiftType(res)
    }
}

impl TryFrom<ParamInfo> for SwiftParam {
    type Error = Error;

    fn try_from(value: ParamInfo) -> Result<Self> {
        let (param_type, wrap_as, deter_as) = if value.ty.tags.iter().any(|t| t == "TW_DATA") {
            (
                SwiftType("Data".to_string()),
                Some(format!("TWDataCreateWithNSData({})", value.name)),
                Some(format!("TWDataDelete({})", value.name)),
            )
        } else if value.ty.tags.iter().any(|t| t == "TW_STRING") {
            (
                SwiftType("String".to_string()),
                Some(format!("TWStringCreateWithNSString({})", value.name)),
                Some(format!("TWStringDelete({})", value.name)),
            )
        } else {
            let wrap_as = match &value.ty.variant {
                TypeVariant::Struct(_) => Some(format!("{}.rawValue", value.name)),
                TypeVariant::Enum(obj_name) => {
                    Some(format!("{obj_name}(rawValue: {}.rawValue)", value.name))
                }
                _ => None,
            };

            (
                SwiftType::try_from(value.ty.variant).unwrap(),
                wrap_as,
                None,
            )
        };

        Ok(SwiftParam {
            name: value.name,
            param_type,
            is_nullable: value.ty.is_nullable,
        })
    }
}

impl TryFrom<ProtoInfo> for SwiftProto {
    type Error = Error;

    fn try_from(value: ProtoInfo) -> std::result::Result<Self, Self::Error> {
        let mut name = value.0.replace("_", "");
        name = name.replace("TW", "");
        name = name.replace("Proto", "");

        Ok(SwiftProto {
            name,
            c_ffi_name: value.0,
        })
    }
}
