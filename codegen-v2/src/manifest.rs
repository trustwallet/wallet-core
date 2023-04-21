use crate::grammar::GHeaderInclude;
use std::convert::TryFrom;

enum Error {
    BadImport,
}

type Result<T> = std::result::Result<T, Error>;

enum TypeInfo {
    Bool,
    Int8,
    Int16,
    Int32,
    Int64,
    Uint8,
    Uint16,
    Uint32,
    Uint64,
    Custom(String),
}

struct FileInfo {
    name: String,
    imports: Vec<ImportInfo>,
    structs: Vec<StructInfo>,
    enums: Vec<EnumInfo>,
}

struct ImportInfo {
    // Expressed as directories plus the final file.
    // E.g. `to/some/file.h` ~= ["to", "some", "file.h"]
    path: Vec<String>,
}

impl TryFrom<GHeaderInclude> for ImportInfo {
    type Error = Error;

    fn try_from(value: GHeaderInclude) -> Result<Self> {
        let path: Vec<String> = value.0.split('/').map(|s| s.to_string()).collect();

        if path.is_empty() {
            return Err(Error::BadImport);
        }

        Ok(ImportInfo { path })
    }
}

struct EnumInfo {
    name: String,
    variants: Vec<(String, Option<usize>)>,
}

struct StructInfo {
    name: String,
    is_public: bool,
    fields: Vec<(String, TypeInfo)>,
    methods: Vec<Method>,
    properties: Vec<PropertyInfo>,
}

struct PropertyInfo {
    name: String,
    is_public: bool,
    is_static: bool,
    return_type: ReturnInfo,
    comments: Vec<String>,
}

struct Params {
    ty: TypeInfo,
    name: String,
    is_pointer: bool,
    is_nullable: bool,
}

struct ReturnInfo {
    ty: TypeInfo,
    is_pointer: bool,
    is_nullable: bool,
}

struct Method {
    name: String,
    is_public: bool,
    is_static: bool,
    return_type: ReturnInfo,
    comments: Vec<String>,
}
