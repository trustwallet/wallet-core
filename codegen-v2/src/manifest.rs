use crate::grammar::{
    GEnumDecl, GFunctionDecl, GHeaderInclude, GMarker, GMarkers, GStructDecl, GType,
};
use std::convert::TryFrom;

enum Error {
    BadImport,
    BadObject,
    BadProperty,
}

type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, PartialEq, Eq)]
struct TypeInfo {
    ty: TypeVariant,
    is_nullable: bool,
    is_pointer: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
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

impl TypeInfo {
    fn from_g_type(ty: &GType, markers: &GMarkers) -> Result<Self> {
        todo!()
    }
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

        // Identify whether it's a struct or a class. The object must be *one*
        // of the two available markers and is always public
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

impl PropertyInfo {
    fn from_g_type(object: &ObjectInfo, value: &GFunctionDecl) -> Result<Self> {
        // ### Name

		// Strip the object name from the property name.
		// E.g. "SomeObjectIsValid" => "IsValid"
        let name = value
            .name
            .0
            .strip_prefix(&object.name)
            .ok_or(Error::BadProperty)?
			.to_string();

        if name.is_empty() {
            return Err(Error::BadProperty);
        }

        // ### Marker

        let mut markers = value.markers.0.iter();

        // Must have one marker.
        if markers.size_hint().0 != 1 {
            return Err(Error::BadProperty);
        }

        // The property must have one of the two available markers and is always public.
        let (is_static, is_public) = match markers.next() {
            Some(GMarker::TwExportProperty) => (false, true),
            Some(GMarker::TwExportStaticProperty) => (true, true),
            _ => return Err(Error::BadObject),
        };

        // ### Param

        // Must have one parameter.
        if value.params.len() != 1 {
            return Err(Error::BadProperty);
        }

        let g_ty = value.params.get(0).unwrap();
        let info = TypeInfo::from_g_type(&g_ty.ty, &g_ty.markers)?;

        // The parameter type must be the same as the object this property
        // belongs to.
        if let TypeVariant::Custom(name) = info.ty {
            if name != object.name {
                return Err(Error::BadProperty);
            }
        } else {
            return Err(Error::BadProperty);
        }

        // Must be a pointer and not nullable.
        if info.is_nullable || !info.is_pointer {
            return Err(Error::BadProperty);
        }

        // ### Return value

        // Extract return value.
        let re = &value.return_value;
        let (re_ty, re_markers) = (&re.ty, &re.markers);
        let return_type = TypeInfo::from_g_type(re_ty, re_markers)?;

        Ok(PropertyInfo {
            name,
            is_public,
            is_static,
            return_type,
            comments: vec![],
        })
    }
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
