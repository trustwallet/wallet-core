// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::Result;
use std::fs;
use std::path::Path;

pub fn parse_dir<P: AsRef<Path>>(path: P) -> Result<Vec<FileInfo>> {
    // Get a list of all files in the directory
    let entries = fs::read_dir(path)?;

    let mut file_infos = vec![];
    for entry in entries {
        let entry = entry?;
        let file_path = entry.path();

        // Skip directories
        if file_path.is_dir() {
            println!("Found unexpected directory: {}", file_path.display());
            continue;
        }

        // Read the file into a string
        let file_contents = fs::read_to_string(&file_path)?;

        // Deserialize the JSON into a struct
        let info = parse_str(&file_contents)?;
        file_infos.push(info);
    }

    Ok(file_infos)
}

pub fn parse_str(str: &str) -> Result<FileInfo> {
    serde_yaml::from_str(str).map_err(|err| err.into())
}

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
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub structs: Vec<StructInfo>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub inits: Vec<InitInfo>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub deinits: Vec<DeinitInfo>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub enums: Vec<EnumInfo>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub functions: Vec<FunctionInfo>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub properties: Vec<PropertyInfo>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
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
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub variants: Vec<EnumVariantInfo>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct EnumVariantInfo {
    pub name: String,
    pub value: String,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub as_string: Option<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct StructInfo {
    pub name: String,
    pub is_public: bool,
    pub is_class: bool,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub fields: Vec<(String, TypeInfo)>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct InitInfo {
    pub name: String,
    pub is_public: bool,
    pub is_nullable: bool,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub params: Vec<ParamInfo>,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
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
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub params: Vec<ParamInfo>,
    pub return_type: TypeInfo,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub comments: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct PropertyInfo {
    pub name: String,
    pub is_public: bool,
    pub return_type: TypeInfo,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub comments: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ParamInfo {
    pub name: String,
    #[serde(rename = "type")]
    pub ty: TypeInfo,
}
