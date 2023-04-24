use crate::grammar::{
    GEnumDecl, GFunctionDecl, GHeaderInclude, GMarker, GMarkers, GPrimitive, GStructDecl, GType,
    GTypeCategory,
};
use crate::manifest::*;

impl TypeInfo {
    pub fn from_g_type(ty: &GType, markers: &GMarkers) -> Result<Self> {
        pub fn get_variant(category: &GTypeCategory) -> Result<TypeVariant> {
            let variant = match category {
                GTypeCategory::Scalar(s) => match s {
                    GPrimitive::Void => TypeVariant::Void,
                    GPrimitive::Bool => TypeVariant::Bool,
                    GPrimitive::Char => TypeVariant::Char,
                    GPrimitive::ShortInt => TypeVariant::ShortInt,
                    GPrimitive::Int => TypeVariant::Int,
                    GPrimitive::UnsignedInt => TypeVariant::UnsignedInt,
                    GPrimitive::LongInt => TypeVariant::LongInt,
                    GPrimitive::Float => TypeVariant::Float,
                    GPrimitive::Double => TypeVariant::Double,
                    GPrimitive::SizeT => TypeVariant::SizeT,
                    GPrimitive::Int8T => TypeVariant::Int8T,
                    GPrimitive::Int16T => TypeVariant::Int16T,
                    GPrimitive::Int32T => TypeVariant::Int32T,
                    GPrimitive::Int64T => TypeVariant::Int64T,
                    GPrimitive::UInt8T => TypeVariant::UInt8T,
                    GPrimitive::UInt16T => TypeVariant::UInt16T,
                    GPrimitive::UInt32T => TypeVariant::UInt32T,
                    GPrimitive::UInt64T => TypeVariant::UInt64T,
                },
                GTypeCategory::Struct(s) => TypeVariant::Struct(s.0 .0.to_string()),
                GTypeCategory::Enum(e) => TypeVariant::Enum(e.0 .0.to_string()),
                GTypeCategory::Pointer(_) | GTypeCategory::Unrecognized(_) => {
                    return Err(Error::BadType);
                }
            };

            Ok(variant)
        }

        pub fn get_variant_pointer_check(cat: &GTypeCategory) -> Result<(TypeVariant, bool)> {
            let res = match cat {
                GTypeCategory::Pointer(p) => (get_variant(&*p)?, true),
                _ => (get_variant(cat)?, false),
            };

            Ok(res)
        }

        let is_nullable = markers.0.iter().any(|m| match m {
            GMarker::Nullable => true,
            GMarker::NonNull => false,
            _ => false,
        });

        if let GType::Mutable(GTypeCategory::Pointer(pointer)) = ty {
            if let GTypeCategory::Unrecognized(ref keyword) = **pointer {
                if keyword.0 == "TWData" {
                    return Ok(TypeInfo {
                        variant: TypeVariant::Void,
                        is_constant: true,
                        is_nullable,
                        is_pointer: true,
                        tags: vec!["TW_DATA".to_string()],
                    });
                } else if keyword.0 == "TWString" {
                    return Ok(TypeInfo {
                        variant: TypeVariant::Void,
                        is_constant: true,
                        is_nullable,
                        is_pointer: true,
                        tags: vec!["TW_STRING".to_string()],
                    });
                }
            }
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
            is_nullable,
            is_pointer,
            tags: vec![],
        })
    }
}

impl ImportInfo {
    pub fn from_g_type(value: &GHeaderInclude) -> Result<Self> {
        let path: Vec<String> = value.0.split('/').map(|s| s.to_string()).collect();

        if path.is_empty() {
            return Err(Error::BadImport);
        }

        Ok(ImportInfo { path })
    }
}

impl EnumInfo {
    pub fn from_g_type(value: &GEnumDecl) -> Result<Self> {
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
            tags: vec![],
        })
    }
}

impl StructInfo {
    pub fn from_g_type(value: &GStructDecl) -> Result<Self> {
        let mut markers = value.markers.0.iter();

        if markers.size_hint().0 != 1 {
            return Err(Error::BadObject);
        }

        // Identify whether it's a struct or a class. The object must be *one*
        // of the two available markers and is always public
        let mut tags = vec![];
        match markers.next() {
            Some(GMarker::TwExportStruct) => {
                tags.push("TW_EXPORT_STRUCT".to_string());
            }
            Some(GMarker::TwExportClass) => {
                tags.push("TW_EXPORT_CLASS".to_string());
            }
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

        Ok(StructInfo {
            name: value.name.0 .0.to_string(),
            is_public: true,
            fields,
            tags,
        })
    }
}

impl PropertyInfo {
    pub fn from_g_type(value: &GFunctionDecl) -> Result<Self> {
        // ### Name

        // Strip the object name from the property name.
        // E.g. "SomeObjectIsValid" => "IsValid"
        let name = value.name.0.clone();

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

        // Must have at least one parameter.
        if value.params.len() < 1 {
            return Err(Error::BadProperty);
        }

        /*
        // Convert GType to TypeInfo.
        let g_ty = value.params.get(0).unwrap();
        let ty = TypeInfo::from_g_type(&g_ty.ty, &g_ty.markers)?;

        // Must be a pointer and not nullable.
        if ty.is_nullable || !ty.is_pointer {
            return Err(Error::BadProperty);
        }
        */

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

impl InitInfo {
    pub fn from_g_type(value: &GFunctionDecl) -> Result<Self> {
        let func = FunctionInfo::from_g_type(&None, value)?;

        Ok(InitInfo {
            name: func.name,
            params: func.params,
            comments: vec![],
        })
    }
}

impl DeinitInfo {
    pub fn from_g_type(value: &GFunctionDecl) -> Result<Self> {
        let func = FunctionInfo::from_g_type(&None, value)?;

        Ok(DeinitInfo {
            name: func.name,
            params: func.params,
            comments: vec![],
        })
    }
}

impl FunctionInfo {
    pub fn from_g_type(object_name: &Option<String>, value: &GFunctionDecl) -> Result<Self> {
        // ### Name

        // Strip the object name from the method name.
        // E.g. "SomeObjectIsValid" => "IsValid"
        let name = if let Some(object_name) = object_name {
            value
                .name
                .0
                .strip_prefix(object_name)
                .ok_or(Error::BadProperty)?
                .to_string()
        } else {
            value.name.0.to_string()
        };

        if name.is_empty() {
            return Err(Error::BadProperty);
        }

        // ### Marker

        let mut markers = value.markers.0.iter();

        // Must have at least one marker.
        if markers.size_hint().0 < 1 {
            return Err(Error::BadProperty);
        }

        // The method must have one of the two available markers and is always public.
        let (is_static, is_public) = match markers.next() {
            Some(GMarker::TwExportMethod) => (false, true),
            Some(GMarker::TwExportStaticMethod) => (true, true),
            _ => return Err(Error::BadObject),
        };

        // ### Params

        let mut g_params = value.params.iter();

        // Convert parameters.
        let mut params = vec![];
        while let Some(g_item) = g_params.next() {
            params.push(ParamInfo {
                name: g_item.name.0.to_string(),
                ty: TypeInfo::from_g_type(&g_item.ty, &g_item.markers)?,
                // TODO
                tags: vec![],
            })
        }

        // ### Return value

        // Extract return value.
        let re = &value.return_value;
        let return_type = TypeInfo::from_g_type(&re.ty, &re.markers)?;

        Ok(FunctionInfo {
            name,
            is_public,
            is_static,
            params,
            return_type,
            comments: vec![],
        })
    }
}
