use crate::grammar::{
    GEnumDecl, GFunctionDecl, GHeaderInclude, GMarker, GMarkers, GPrimitive, GStructDecl, GType,
    GTypeCategory,
};

enum Error {
    BadImport,
    BadObject,
    BadProperty,
    BadType,
}

type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, PartialEq, Eq)]
struct TypeInfo {
    variant: TypeVariant,
    is_constant: bool,
    is_nullable: bool,
    is_pointer: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
enum TypeVariant {
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
    Custom(String),
}

impl TypeInfo {
    fn from_g_type(ty: &GType, markers: &GMarkers) -> Result<Self> {
        fn get_variant(category: &GTypeCategory) -> Result<TypeVariant> {
            let variant = match category {
                GTypeCategory::Scalar(s) => match s {
                    GPrimitive::Void => TypeVariant::Void,
                    GPrimitive::Bool => TypeVariant::Bool,
                    GPrimitive::Char => TypeVariant::Int8,
                    GPrimitive::ShortInt => TypeVariant::Int16,
                    GPrimitive::Int => TypeVariant::Int32,
                    GPrimitive::UnsignedInt => TypeVariant::Uint32,
                    GPrimitive::LongInt => TypeVariant::Int64,
                    GPrimitive::Float => TypeVariant::Float32,
                    GPrimitive::Double => TypeVariant::Float64,
                    GPrimitive::UInt8T => TypeVariant::Uint8,
                    GPrimitive::UInt32T => TypeVariant::Uint32,
                },
                GTypeCategory::Struct(s) => TypeVariant::Struct(s.0 .0.to_string()),
                GTypeCategory::Enum(e) => TypeVariant::Enum(e.0 .0.to_string()),
                // Unrecognized types and multi-pointers are not supported
                GTypeCategory::Pointer(_) | GTypeCategory::Unrecognized(_) => {
                    return Err(Error::BadType);
                }
            };

            Ok(variant)
        }

        fn get_variant_pointer_check(cat: &GTypeCategory) -> Result<(TypeVariant, bool)> {
            let res = match cat {
                GTypeCategory::Pointer(p) => (get_variant(&*p)?, true),
                _ => (get_variant(cat)?, false),
            };

            Ok(res)
        }

        let ((variant, is_pointer), is_constant) = match ty {
            GType::Mutable(category) => (get_variant_pointer_check(category)?, false),
            GType::Const(category) => (get_variant_pointer_check(category)?, true),
            GType::Extern(_) => {
                return Err(Error::BadType);
                // TODO: Where should this be handled?
            }
        };

        Ok(TypeInfo {
            variant,
            is_constant,
            is_nullable: false,
            is_pointer,
        })
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

impl ImportInfo {
    fn from_g_type(value: &GHeaderInclude) -> Result<Self> {
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

impl EnumInfo {
    fn from_g_type(value: &GEnumDecl) -> Result<Self> {
        Ok(EnumInfo {
            name: value.name.0.to_string(),
			// Enums are always public
            // TOOD: Should be part of GEnumDecl
            is_public: true,
            variants: value
                .variants
                .iter()
                .cloned()
                .map(|(k, v)| (k.0, v))
                .collect(),
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

impl ObjectInfo {
    fn from_g_type(value: &GStructDecl) -> Result<Self> {
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
            .iter()
            .map(|(k, v)| {
                Ok((
                    k.0.to_string(),
                    // In this context, types inside a C struct do not support
                    // markers.
                    TypeInfo::from_g_type(v, &GMarkers(vec![]))?,
                ))
            })
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

        // Convert GType to TypeInfo.
        let g_ty = value.params.get(0).unwrap();
        let ty = TypeInfo::from_g_type(&g_ty.ty, &g_ty.markers)?;

        // The parameter type must be the same as the object this property
        // belongs to.
        if let TypeVariant::Custom(name) = ty.variant {
            if name != object.name {
                return Err(Error::BadProperty);
            }
        } else {
            return Err(Error::BadProperty);
        }

        // Must be a pointer and not nullable.
        if ty.is_nullable || !ty.is_pointer {
            return Err(Error::BadProperty);
        }

        // ### Return value

        // Extract return value.
        let re = &value.return_value;
        let return_type = TypeInfo::from_g_type(&re.ty, &re.markers)?;

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
