use std::fmt::Display;

use self::functions::process_object_methods;
use self::inits::process_inits;
use self::properties::process_object_properties;
use crate::manifest::{FileInfo, ProtoInfo, TypeVariant};
use handlebars::Handlebars;
use serde_json::json;

mod functions;
mod inits;
mod properties;

type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub enum Error {
    InvalidType,
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

        // Stip "TW" prefix if present.
        let struct_name = strct
            .name
            .strip_prefix("TW")
            .unwrap_or(&strct.name)
            .to_string();

        // Add superclasses.
        let superclasses = if struct_name.ends_with("Address") {
            vec!["Address"]
        } else {
            vec![]
        };

        // Handle equality operator.
        let equality_method = methods.iter().enumerate().find(|(_, f)| f.name == "equal");

        let equality_operator = if let Some((idx, _func)) = equality_method {
            let operator = SwiftOperatorEquality {
                c_ffi_name: format!("{}Equal", strct.name),
            };

            // Remove that method from the `methods` list.
            methods.remove(idx);

            Some(operator)
        } else {
            None
        };

        let payload = json!({
            "name": struct_name,
            "is_class": strct.is_class,
            "init_instance": strct.is_class,
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

        // Stip "TW" prefix if present.
        let enum_name = enm.name.strip_prefix("TW").unwrap_or(&enm.name).to_string();

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
        let file_name = info
            .name
            .strip_prefix("TW")
            .unwrap_or(&info.name)
            .strip_suffix("Proto")
            .unwrap_or(&info.name)
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

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SwiftType(String);

impl Display for SwiftType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

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
    CallOptional {
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
                // We strip the "TW" prefix for Swift representations of
                // structs/enums.
                n.strip_prefix("TW").map(|n| n.to_string()).unwrap_or(n)
            }
        };

        SwiftType(res)
    }
}
