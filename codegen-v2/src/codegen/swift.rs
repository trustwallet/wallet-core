use crate::grammar::{GFunctionDecl, GKeyword, GMarker, GPrimitive, GType, GTypeCategory};
use crate::manifest::{TypeVariant, FunctionInfo, FileInfo};
use crate::{Error, Result};
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
    param_type: String,
    is_nullable: bool,
    wrap_as: Option<String>,
    deter_as: Option<String>,
}

#[derive(Serialize, Deserialize)]
pub struct SwiftReturn {
    pub name: String,
    #[serde(rename = "type")]
    pub ty: SwiftType,
}

struct SwiftTypeContext {
    name: String,
    ty: String,
    wrap_as: Option<String>,
    deter_as: Option<String>,
}

// Converts the parsed grammar `GType` into the corresponding Swift type,
// including how it should be wrapped and/or deterred. The types must be
// specifically matched/supported, otherwise an error is returned.
fn get_type_str(gty: &GType) -> Result<SwiftTypeContext> {
    // Match `Uint8T` primitive.
    if let GType::Mutable(GTypeCategory::Scalar(GPrimitive::UInt8T)) = gty {
        return Ok(SwiftTypeContext {
            name: "int".to_string(),
            ty: "UInt8".to_string(),
            wrap_as: None,
            deter_as: None,
        });
    }

    if let GType::Mutable(GTypeCategory::Pointer(boxed)) = gty {
        if let GTypeCategory::Unrecognized(ref keyword) = **boxed {
            if keyword.0 == "TWString" {
                return Ok(SwiftTypeContext {
                    name: "string".to_string(),
                    ty: "String".to_string(),
                    wrap_as: Some("TWStringCreateWithNSString(string)".to_string()),
                    deter_as: Some("StringDelete(string)".to_string()),
                });
            } else if keyword.0 == "TWData" {
                return Ok(SwiftTypeContext {
                    name: "data".to_string(),
                    ty: "Data".to_string(),
                    wrap_as: Some("TWDataCreateWithNSData(data)".to_string()),
                    deter_as: Some("TWDataDelete(data)".to_string()),
                });
            }
        }
    }

    Err(Error::Todo)
}

fn get_method_name(prefix: &GKeyword, keyword: &GKeyword) -> Result<String> {
    let prefix = &prefix.0;
    let name = &keyword.0;

    // Failure here would imply a bug. This should be catched before.
    // TODO: Write where.
    let name = name.strip_prefix(prefix).ok_or(Error::Todo)?;

    if name.is_empty() {
        return Err(Error::Todo);
    }

    // Lowercase first letter.
    let mut name = name.to_string();
    let name = name.remove(0).to_lowercase().to_string() + &name;

    Ok(name)
}

fn process_file_info(info: &FileInfo) {

}
