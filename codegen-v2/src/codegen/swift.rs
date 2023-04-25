use crate::grammar::{GFunctionDecl, GKeyword, GMarker, GPrimitive, GType, GTypeCategory};
use crate::manifest::{FileInfo, FunctionInfo, ParamInfo, TypeInfo, TypeVariant};
use crate::{parse, Error, Result};
use handlebars::Handlebars;

pub const METHOD_INFO: &str = "part_method.hbs";

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

impl TryFrom<TypeInfo> for SwiftReturn {
    type Error = Error;

    fn try_from(value: TypeInfo) -> std::result::Result<Self, Self::Error> {
        let (param_type, wrap_as, deter_as) = if value.tags.iter().any(|t| t == "TW_DATA") {
            (
                SwiftType("Data".to_string()),
                Some("TWDataCreateWithNSData(result)".to_string()),
                Some("TWDataDelete(result)".to_string()),
            )
        } else if value.tags.iter().any(|t| t == "TW_STRING") {
            (
                SwiftType("String".to_string()),
                Some("TWStringCreateWithNSString(result)".to_string()),
                Some("StringDelete(result)".to_string()),
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
                Some("TWDataCreateWithNSData(data)".to_string()),
                Some("TWDataDelete(data)".to_string()),
            )
        } else if value.ty.tags.iter().any(|t| t == "TW_STRING") {
            (
                SwiftType("String".to_string()),
                Some("TWStringCreateWithNSString(string)".to_string()),
                Some("StringDelete(string)".to_string()),
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

fn process_func(struct_names: &[&str], func: FunctionInfo) -> Result<SwiftFunction> {
    let mut func_name = None;

    for struct_name in struct_names {
        if let Some(suffix) = func.name.strip_prefix(struct_name) {
            // We do not allow this.
            if func_name.is_some() {
                panic!()
            }

            func_name = Some(suffix.to_string());
        }
    }

    let func_name = if func_name.is_some() {
        func_name.unwrap()
    } else {
        func.name.clone()
    };

    // Convert parameters.
    let mut params = vec![];
    for param in func.params {
        params.push(SwiftParam::try_from(param).unwrap());
    }

    // Convert return type.
    let return_type = SwiftReturn::try_from(func.return_type).unwrap();

    Ok(SwiftFunction {
        name: func_name,
        c_ffi_name: func.name.clone(),
        is_public: func.is_public,
        is_static: func.is_static,
        params,
        return_type,
        comments: vec![],
    })
}

fn process_file_info(info: FileInfo) {
    let struct_names: Vec<&str> = info.structs.iter().map(|s| s.name.as_ref()).collect();

    for func in info.functions {
        let func = process_func(&struct_names, func).unwrap();

        let mut engine = Handlebars::new();
        engine.set_strict_mode(true);

        engine
            .register_template_file(METHOD_INFO, template_path(METHOD_INFO))
            .unwrap();

        let out = engine.render(METHOD_INFO, &func).unwrap();

        println!("{}", out);

        break;
    }
}

const TEMPLATE_DIR: &str = "src/codegen/templates/swift/";

fn template_path(file: &str) -> String {
    format!("{}{}", TEMPLATE_DIR, file)
}

#[test]
#[ignore]
fn test_swift_template() {
    let path = std::path::Path::new("../include/");
    let dir = crate::parse(&path).unwrap();
    let file_infos = crate::manifest::process_c_header_dir(&dir);

    for file_info in file_infos {
        process_file_info(file_info);
    }
}
