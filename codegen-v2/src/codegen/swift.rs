use crate::manifest::{
    FileInfo, FunctionInfo, InitInfo, ParamInfo, PropertyInfo, TypeInfo, TypeVariant,
};
use crate::{Error, Result};
use handlebars::Handlebars;
use serde_json::json;

#[derive(Serialize, Deserialize)]
pub struct SwiftType(String);

#[derive(Serialize, Deserialize)]
pub struct SwiftFunction {
    pub name: String,
    pub c_ffi_name: String,
    pub is_public: bool,
    pub is_static: bool,
    pub params: Vec<SwiftParam>,
    #[serde(rename = "return")]
    pub return_type: SwiftReturn,
    pub comments: Vec<String>,
}

#[derive(Serialize, Deserialize)]
struct SwiftProperty {
    pub name: String,
    pub c_ffi_name: String,
    pub is_public: bool,
    pub is_static: bool,
    #[serde(rename = "return")]
    pub return_type: SwiftReturn,
    pub comments: Vec<String>,
}

#[derive(Serialize, Deserialize)]
pub struct SwiftParam {
    pub name: String,
    #[serde(rename = "type")]
    pub param_type: SwiftType,
    pub is_nullable: bool,
    pub skip_self: bool,
    pub wrap_as: Option<String>,
    pub deter_as: Option<String>,
}

#[derive(Serialize, Deserialize)]
pub struct SwiftReturn {
    #[serde(rename = "type")]
    pub param_type: SwiftType,
    pub is_nullable: bool,
    pub wrap_as: Option<String>,
    pub deter_as: Option<String>,
}

#[derive(Serialize, Deserialize)]
pub struct SwiftInit {
    pub name: String,
    pub c_ffi_name: String,
    pub is_public: bool,
    pub params: Vec<SwiftParam>,
    pub comments: Vec<String>,
}

pub struct RenderIntput<'a> {
    pub file_info: FileInfo,
    pub struct_template: &'a str,
    pub enum_template: &'a str,
    pub extension_template: &'a str,
}

#[derive(Default)]
pub struct RenderOutput {
    pub structs: Vec<(String, String)>,
    pub enums: Vec<(String, String)>,
    pub extensions: Vec<(String, String)>,
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

    let mut info = input.file_info;
    let mut outputs = RenderOutput::default();

    for strct in info.structs {
        let is_class = strct.tags.iter().any(|t| t == "TW_EXPORT_CLASS");

        let (inits, methods, properties);
        (inits, info.inits) = process_inits(&strct.name, info.inits).unwrap();
        (methods, info.functions) = process_struct_methods(&strct.name, info.functions).unwrap();
        (properties, info.properties) =
            process_struct_properties(&strct.name, info.properties).unwrap();

        // TODO: Extend
        let payload = json!({
            "name": strct.name.strip_prefix("TW").ok_or(Error::Todo)?,
            "is_class": is_class,
            "init_instance": true,
            "parent_classes": [],
            "inits": inits,
            "deinits": [],
            "methods": methods,
            "properties": properties,
        });

        let out = engine.render("struct", &payload).unwrap();

        outputs.structs.push((strct.name.to_string(), out));
    }

    for enm in info.enums {
        let (methods, properties);
        (methods, info.functions) = process_struct_methods(&enm.name, info.functions).unwrap();
        (properties, info.properties) =
            process_struct_properties(&enm.name, info.properties).unwrap();

        let enum_name = enm.name.strip_prefix("TW").ok_or(Error::Todo)?;

        // TODO: Extend
        let enum_payload = json!({
            "name": enum_name,
            "parent_classes": [],
            "variants": enm.variants,
        });

        let extension_payload = json!({
            "name": enum_name,
            "init_instance": true,
            "parent_classes": [],
            "methods": methods,
            "properties": properties,
        });

        let out = engine.render("enum", &enum_payload).unwrap();
        outputs.enums.push((enm.name.to_string(), out));

        let out = engine.render("extension", &extension_payload).unwrap();
        outputs.extensions.push((enm.name.to_string(), out));
    }

    Ok(outputs)
}

fn process_inits(
    object_name: &str,
    inits: Vec<InitInfo>,
) -> Result<(Vec<SwiftInit>, Vec<InitInfo>)> {
    let mut swift_inits = vec![];
    let mut info_inits = vec![];

    for init in inits {
        if !init.name.starts_with(object_name) {
            // Init is not assciated with the object.
            info_inits.push(init);
            continue;
        }

        let init_name = init.name.strip_prefix(object_name).unwrap().to_string();

        // Convert parameters.
        let params = init
            .params
            .into_iter()
            .map(|p| SwiftParam::try_from(p))
            .collect::<Result<Vec<SwiftParam>>>()?;

        swift_inits.push(SwiftInit {
            name: init_name,
            c_ffi_name: init.name.clone(),
            is_public: init.is_public,
            params,
            comments: vec![],
        });
    }

    Ok((swift_inits, info_inits))
}

fn process_struct_methods(
    object_name: &str,
    functions: Vec<FunctionInfo>,
) -> Result<(Vec<SwiftFunction>, Vec<FunctionInfo>)> {
    let mut swift_funcs = vec![];
    let mut info_funcs = vec![];

    for func in functions {
        if !func.name.starts_with(object_name) {
            // Function is not assciated to the struct.
            info_funcs.push(func);
            continue;
        }

        let func_name = func.name.strip_prefix(object_name).unwrap().to_string();

        // Convert parameters.
        let mut params = vec![];
        for param in func.params {
            // Skip self-referencing struct paramater for non-static methods.
            if let TypeVariant::Struct(ref struct_name) = param.ty.variant {
                if struct_name == object_name && !func.is_static {
                    continue;
                }
            }

            // Skip self-referencing enum parameter and wrap accordingly
            let (mut wrap_as, mut skip_self) = (None, false);
            if let TypeVariant::Enum(ref enum_name) = param.ty.variant {
                if enum_name == object_name {
                    wrap_as = Some(format!("{}(rawValue: rawValue)", enum_name));
                    skip_self = true;
                }
            }

            let mut swift_param = SwiftParam::try_from(param).unwrap();
            swift_param.wrap_as = wrap_as;
            swift_param.skip_self = skip_self;

            params.push(swift_param);
        }

        // Convert return type.
        let return_type = SwiftReturn::try_from(func.return_type).unwrap();

        swift_funcs.push(SwiftFunction {
            name: func_name,
            c_ffi_name: func.name.clone(),
            is_public: func.is_public,
            is_static: func.is_static,
            params,
            return_type,
            comments: vec![],
        });
    }

    Ok((swift_funcs, info_funcs))
}

fn process_struct_properties(
    object_name: &str,
    properties: Vec<PropertyInfo>,
) -> Result<(Vec<SwiftProperty>, Vec<PropertyInfo>)> {
    let mut swift_props = vec![];
    let mut info_props = vec![];

    for prop in properties {
        if !prop.name.starts_with(object_name) {
            // Function is not assciated to the struct.
            info_props.push(prop);
            continue;
        }

        let prop_name = prop.name.strip_prefix(object_name).unwrap().to_string();

        // Convert return type.
        let return_type = SwiftReturn::try_from(prop.return_type).unwrap();

        swift_props.push(SwiftProperty {
            name: prop_name,
            c_ffi_name: prop.name.clone(),
            is_public: prop.is_public,
            is_static: prop.is_static,
            return_type,
            comments: vec![],
        });
    }

    Ok((swift_props, info_props))
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
            TypeVariant::Struct(n) | TypeVariant::Enum(n) => n,
        };

        SwiftType(res)
    }
}

impl TryFrom<TypeInfo> for SwiftReturn {
    type Error = Error;

    fn try_from(value: TypeInfo) -> std::result::Result<Self, Self::Error> {
        let (param_type, wrap_as, deter_as) = if value.tags.iter().any(|t| t == "TW_DATA") {
            (
                SwiftType("Data".to_string()),
                Some("TWDataCreateWithNSData".to_string()),
                Some("TWDataDelete".to_string()),
            )
        } else if value.tags.iter().any(|t| t == "TW_STRING") {
            (
                SwiftType("String".to_string()),
                Some("TWStringCreateWithNSString".to_string()),
                Some("StringDelete".to_string()),
            )
        } else {
            (SwiftType::try_from(value.variant).unwrap(), None, None)
        };

        Ok(SwiftReturn {
            param_type,
            is_nullable: value.is_nullable,
            wrap_as,
            deter_as,
        })
    }
}

impl TryFrom<ParamInfo> for SwiftParam {
    type Error = Error;

    fn try_from(value: ParamInfo) -> Result<Self> {
        let (param_type, wrap_as, deter_as) = if value.ty.tags.iter().any(|t| t == "TW_DATA") {
            (
                SwiftType("Data".to_string()),
                Some("TWDataCreateWithNSData".to_string()),
                Some("TWDataDelete".to_string()),
            )
        } else if value.ty.tags.iter().any(|t| t == "TW_STRING") {
            (
                SwiftType("String".to_string()),
                Some("TWStringCreateWithNSString".to_string()),
                Some("TWStringDelete".to_string()),
            )
        } else {
            (SwiftType::try_from(value.ty.variant).unwrap(), None, None)
        };

        Ok(SwiftParam {
            name: value.name,
            param_type,
            is_nullable: value.ty.is_nullable,
            skip_self: false,
            wrap_as: wrap_as,
            deter_as: deter_as,
        })
    }
}
