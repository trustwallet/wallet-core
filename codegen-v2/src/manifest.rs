use crate::grammar::{GEnumDecl, GHeaderInclude, GMarker, GStructDecl, GType};
use std::convert::TryFrom;

enum Error {
    BadImport,
    BadObject,
}

type Result<T> = std::result::Result<T, Error>;

struct TypeInfo {
	ty: TypeVariant,
	is_nullable: bool,
	is_pointer: bool,
}

enum TypeVariant {
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

impl TryFrom<GType> for TypeInfo {
    type Error = Error;

    fn try_from(value: GType) -> Result<Self> {
        todo!()
    }
}

struct FileInfo {
    name: String,
    imports: Vec<ImportInfo>,
    objects: Vec<ObjectInfo>,
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
    is_public: bool,
    variants: Vec<(String, Option<usize>)>,
}

impl TryFrom<GEnumDecl> for EnumInfo {
    type Error = Error;

    fn try_from(value: GEnumDecl) -> Result<Self> {
        Ok(EnumInfo {
            name: value.name.0,
            // TOOD: Should be part of GEnumDecl
            is_public: true,
            variants: value.variants.into_iter().map(|(k, v)| (k.0, v)).collect(),
        })
    }
}

enum ObjectVariant {
    Struct,
    Class,
}

struct ObjectInfo {
    name: String,
    is_public: bool,
    variant: ObjectVariant,
    fields: Vec<(String, TypeInfo)>,
    methods: Vec<Method>,
    properties: Vec<PropertyInfo>,
}

impl TryFrom<GStructDecl> for ObjectInfo {
    type Error = Error;

    fn try_from(value: GStructDecl) -> Result<Self> {
        let mut markers = value.markers.0.iter();

        // The object must have exactly one marker.
        if markers.size_hint().0 != 1 {
            return Err(Error::BadObject);
        }

        // Identify whether it's a struct or a class.
		// The object is always public
        let (variant, is_public) = match markers.next() {
            Some(GMarker::TwExportStruct) => (ObjectVariant::Struct, true),
            Some(GMarker::TwExportClass) => (ObjectVariant::Class, true),
            _ => return Err(Error::BadObject),
        };

		// Process fields.
        let fields = value
            .fields
            .into_iter()
            .map(|(k, v)| Ok((k.0, TypeInfo::try_from(v)?)))
            .collect::<Result<Vec<(String, TypeInfo)>>>()?;

        Ok(ObjectInfo {
            name: value.name.0 .0.to_string(),
            is_public,
            variant,
            fields,
            methods: vec![],
            properties: vec![],
        })
    }
}

struct PropertyInfo {
    name: String,
    is_public: bool,
    is_static: bool,
    return_type: TypeInfo,
    comments: Vec<String>,
}

struct ParamInfo {
    name: String,
    ty: TypeInfo,
}

struct Method {
    name: String,
    is_public: bool,
    is_static: bool,
	params: Vec<ParamInfo>,
    return_type: TypeInfo,
    comments: Vec<String>,
}
