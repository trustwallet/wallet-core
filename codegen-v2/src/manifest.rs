use crate::grammar::CHeaderDirectory;
use crate::grammar::{GHeaderFileItem, GMarker};

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
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub tags: Vec<String>,
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
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub tags: Vec<String>,
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
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub fields: Vec<(String, TypeInfo)>,
    #[serde(skip_serializing_if = "Vec::is_empty")]
    pub tags: Vec<String>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct InitInfo {
    pub name: String,
    pub is_public: bool,
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
    pub is_static: bool,
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

// NOTE: This function is temporary
pub fn process_c_grammar(dir: &CHeaderDirectory) -> Vec<FileInfo> {
    let mut file_infos = vec![];

    for (path, items) in &dir.map {
        let file_name = path
            .file_name()
            .unwrap()
            .to_str()
            .unwrap()
            .strip_suffix(".h")
            .unwrap()
            .to_string();

        let mut file_info = FileInfo {
            name: file_name.clone(),
            structs: vec![],
            inits: vec![],
            deinits: vec![],
            enums: vec![],
            functions: vec![],
            properties: vec![],
            protos: vec![],
        };

        for item in items {
            match item {
                GHeaderFileItem::Typedef(decl) => {
                    if decl.name.contains("Proto") {
                        let x = ProtoInfo::from_g_type(decl).unwrap();
                        file_info.protos.push(x);
                    }
                }
                GHeaderFileItem::StructIndicator(decl) => {
                    let markers = &decl.markers.0;

                    let mut tags = vec![];
                    match markers.first() {
                        Some(GMarker::TwExportStruct) => {
                            tags.push("TW_EXPORT_STRUCT".to_string());
                        }
                        Some(GMarker::TwExportClass) => {
                            tags.push("TW_EXPORT_CLASS".to_string());
                        }
                        _ => {}
                    };

                    file_info.structs.push(StructInfo {
                        name: decl.name.0 .0.clone(),
                        is_public: true,
                        fields: vec![],
                        tags,
                    });
                }
                GHeaderFileItem::StructDecl(decl) => {
                    let x = StructInfo::from_g_type(decl).unwrap();
                    file_info.structs.push(x);
                }
                GHeaderFileItem::EnumDecl(decl) => {
                    let x = EnumInfo::from_g_type(decl).unwrap();
                    file_info.enums.push(x);
                }
                GHeaderFileItem::FunctionDecl(decl) => {
                    if decl.name.0.starts_with("TWAnySigner") {
                        println!(
                            "Skipped function from manifest (non-export):  {}",
                            decl.name.0
                        );
                        continue;
                    }

                    let markers = &decl.markers.0;

                    // Handle exported properties.
                    if markers.contains(&GMarker::TwExportProperty)
                        || markers.contains(&GMarker::TwExportStaticProperty)
                    {
                        let x = PropertyInfo::from_g_type(decl).unwrap();
                        file_info.properties.push(x);
                    }
                    // None-exported methods are skipped.
                    else {
                        // Detect constructor methods.
                        if decl.name.0.contains("Create") {
                            let x = InitInfo::from_g_type(decl).unwrap();
                            file_info.inits.push(x);
                        }
                        // Delect deconstructor methods.
                        else if decl.name.0.contains("Delete") {
                            let x = DeinitInfo::from_g_type(decl).unwrap();
                            file_info.deinits.push(x);
                        }
                        // Any any other method is just a method.
                        else {
                            let x = FunctionInfo::from_g_type(&None, decl).unwrap();
                            file_info.functions.push(x);
                        }
                    }
                }
                _ => {}
            }
        }

        file_infos.push(file_info);
    }

    file_infos
}
