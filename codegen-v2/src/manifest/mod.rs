use crate::grammar::CHeaderDirectory;
use crate::grammar::{GHeaderFileItem, GMarker};

mod manifest_impl;
mod manifest_impl_custom;
pub use manifest_impl::process_c_grammar;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum Error {
    BadImport,
    BadObject,
    BadProperty,
    BadType,
}

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct TypeInfo {
    #[serde(flatten)]
    pub variant: TypeVariant,
    pub is_constant: bool,
    pub is_nullable: bool,
    pub is_pointer: bool,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
#[serde(tag = "variant", content = "value", rename_all = "snake_case")]
pub enum TypeVariant {
    Void,
    Bool,
    Char,
    ShortInt,
    Int,
    UnsignedInt,
    LongInt,
    Float,
    Double,
    SizeT,
    Int8T,
    Int16T,
    Int32T,
    Int64T,
    UInt8T,
    UInt16T,
    UInt32T,
    UInt64T,
    Struct(String),
    Enum(String),
    Data,
    String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct FileInfo {
    pub name: String,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub structs: Vec<StructInfo>,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub inits: Vec<InitInfo>,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub deinits: Vec<DeinitInfo>,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub enums: Vec<EnumInfo>,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub functions: Vec<FunctionInfo>,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub properties: Vec<PropertyInfo>,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub protos: Vec<ProtoInfo>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ImportInfo {
    // Expressed as directories plus the final file.
    // E.g. `to/some/file.h` ~= ["to", "some", "file.h"]
    pub path: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ProtoInfo(pub String);

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct EnumInfo {
    pub name: String,
    pub is_public: bool,
    pub value_type: TypeVariant,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub variants: Vec<EnumVariantInfo>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct EnumVariantInfo {
    pub name: String,
    pub value: usize,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub as_string: Option<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct StructInfo {
    pub name: String,
    pub is_public: bool,
    pub is_class: bool,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub fields: Vec<(String, TypeInfo)>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct InitInfo {
    pub name: String,
    pub is_public: bool,
    pub is_nullable: bool,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub params: Vec<ParamInfo>,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub comments: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct DeinitInfo {
    pub name: String,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct FunctionInfo {
    pub name: String,
    pub is_public: bool,
    pub is_static: bool,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub params: Vec<ParamInfo>,
    pub return_type: TypeInfo,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub comments: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct PropertyInfo {
    pub name: String,
    pub is_public: bool,
    pub return_type: TypeInfo,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub comments: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ParamInfo {
    pub name: String,
    #[serde(rename = "type")]
    pub ty: TypeInfo,
}
