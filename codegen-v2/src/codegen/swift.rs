use std::fmt::format;
use std::io::read_to_string;

use crate::grammar::{GFunctionDecl, GKeyword, GMarker, GPrimitive, GType, GTypeCategory};
use crate::manifest::{
    FileInfo, FunctionInfo, InitInfo, ParamInfo, PropertyInfo, StructInfo, TypeInfo, TypeVariant,
};
use crate::{parse, Error, Result};
use handlebars::{Handlebars, no_escape};
use serde_json::json;

#[derive(Serialize, Deserialize)]
pub struct SwiftType(String);

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
    name: String,
    #[serde(rename = "type")]
    param_type: SwiftType,
    is_nullable: bool,
    wrap_as: Option<String>,
    deter_as: Option<String>,
}

#[derive(Serialize, Deserialize)]
pub struct SwiftReturn {
    #[serde(rename = "type")]
    param_type: SwiftType,
    is_nullable: bool,
    wrap_as: Option<String>,
    deter_as: Option<String>,
}

#[derive(Serialize, Deserialize)]
pub struct SwiftInit {
    name: String,
    c_ffi_name: String,
    is_public: bool,
    params: Vec<SwiftParam>,
    comments: Vec<String>,
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
            wrap_as: wrap_as,
            deter_as: deter_as,
        })
    }
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
    object: &StructInfo,
    functions: Vec<FunctionInfo>,
) -> Result<(Vec<SwiftFunction>, Vec<FunctionInfo>)> {
    let mut swift_funcs = vec![];
    let mut info_funcs = vec![];

    for func in functions {
        if !func.name.starts_with(&object.name) {
            // Function is not assciated to the struct.
            info_funcs.push(func);
            continue;
        }

        let func_name = func.name.strip_prefix(&object.name).unwrap().to_string();

        // Convert parameters.
        let mut params = vec![];
        for param in func.params {
            // Skip struct paramater for non-static methods.
            if let TypeVariant::Struct(ref s) = param.ty.variant {
                if s == &object.name && !func.is_static {
                    continue;
                }
            }

            params.push(SwiftParam::try_from(param).unwrap());
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
    object: &StructInfo,
    properties: Vec<PropertyInfo>,
) -> Result<(Vec<SwiftProperty>, Vec<PropertyInfo>)> {
    let mut swift_props = vec![];
    let mut info_props = vec![];

    for prop in properties {
        if !prop.name.starts_with(&object.name) {
            // Function is not assciated to the struct.
            info_props.push(prop);
            continue;
        }

        let prop_name = prop.name.strip_prefix(&object.name).unwrap().to_string();

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

pub struct RenderConfig {
    pub out_dir: String,
    pub file_template: String,
    pub init_template: String,
    pub method_template: String,
    pub property_template: String,
}

pub fn render_file_info(config: &RenderConfig, mut info: FileInfo) -> Result<()> {
    let mut engine = Handlebars::new();
    // Unmatched variables should result in an error.
    engine.set_strict_mode(true);

    std::fs::create_dir_all(&config.out_dir).unwrap();

    engine
        .register_partial("file", no_escape(&config.file_template))
        .unwrap();

    let mut classes = vec![];

    for object in info.structs {
        let (inits, methods, properties);
        (inits, info.inits) = process_inits(&object.name, info.inits).unwrap();
        (methods, info.functions) = process_struct_methods(&object, info.functions).unwrap();
        (properties, info.properties) =
            process_struct_properties(&object, info.properties).unwrap();

        let payload = json!({
            "class_name": object.name,
            "init_instance": true,
            "parent_classes": [],
            "inits": inits,
            "deinits": [],
            "methods": methods,
            "properties": properties,
        });

        classes.push(payload);
    }

    let file_path = format!("{}/{}.swift", config.out_dir, info.name);

    let out = engine
        .render("file", &json!({ "classes": classes }))
        .unwrap();
    std::fs::write(&file_path, out.as_bytes()).unwrap();

    Ok(())
}

#[test]
#[ignore]
fn test_swift_template() {
    use std::fs::read_to_string;

    let path = std::path::Path::new("../include/");
    let dir = crate::parse(&path).unwrap();
    let file_infos = crate::manifest::process_c_header_dir(&dir);

    let config = RenderConfig {
        out_dir: "out/swift/".to_string(),
        file_template: read_to_string("src/codegen/templates/swift/file.hbs").unwrap(),
        init_template: read_to_string("src/codegen/templates/swift/part_init.hbs").unwrap(),
        method_template: read_to_string("src/codegen/templates/swift/part_method.hbs").unwrap(),
        property_template: read_to_string("src/codegen/templates/swift/part_property.hbs").unwrap(),
    };

    for file_info in file_infos {
        render_file_info(&config, file_info).unwrap();
    }
}
