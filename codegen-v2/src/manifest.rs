pub enum Error {
    BadImport,
    BadObject,
    BadProperty,
    BadType,
}

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct TypeInfo {
    pub variant: TypeVariant,
    pub is_constant: bool,
    pub is_nullable: bool,
    pub is_pointer: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum TypeVariant {
    Void,
    Bool,
    Int8,
    Int16,
    Int32,
    Int64,
    Uint8,
    Uint16,
    Uint32,
    Uint64,
    Float32,
    Float64,
    Struct(String),
    Enum(String),
}

pub struct FileInfo {
    pub name: String,
    pub imports: Vec<ImportInfo>,
    pub objects: Vec<ObjectInfo>,
    pub enums: Vec<EnumInfo>,
}

pub struct ImportInfo {
    // Expressed as directories plus the final file.
    // E.g. `to/some/file.h` ~= ["to", "some", "file.h"]
    pub path: Vec<String>,
}

pub struct EnumInfo {
    pub name: String,
    pub is_public: bool,
    pub variants: Vec<(String, Option<usize>)>,
}

pub struct ObjectInfo {
    pub name: String,
    pub is_public: bool,
    pub variant: ObjectVariant,
    pub fields: Vec<(String, TypeInfo)>,
    pub methods: Vec<MethodInfo>,
    pub properties: Vec<PropertyInfo>,
}

pub enum ObjectVariant {
    Struct,
    Class,
}

pub struct PropertyInfo {
    pub name: String,
    pub is_public: bool,
    pub is_static: bool,
    pub return_type: TypeInfo,
    pub comments: Vec<String>,
}

pub struct MethodInfo {
    pub name: String,
    pub is_public: bool,
    pub is_static: bool,
    pub params: Vec<ParamInfo>,
    pub return_type: TypeInfo,
    pub comments: Vec<String>,
}

pub struct ParamInfo {
    pub name: String,
    pub ty: TypeInfo,
}
