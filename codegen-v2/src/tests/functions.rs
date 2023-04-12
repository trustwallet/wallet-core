use crate::grammar::{
    GFuncName, GFunctionDecl, GMarker, GMarkers, GParamItem, GParamName, GPrimitive, GReturnValue,
    GStruct, GType, GTypeCategory, ParseTree,
};
use crate::reader::Reader;

#[test]
fn test_func_params_with_marker() {
    let driver = Reader::from("int _Nonnull my_var\n");
    let der = GParamItem::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItem {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
            name: GParamName::from("my_var"),
            markers: GMarkers(vec![GMarker::NonNull]),
        }
    );

    let driver = Reader::from("bool\n_Nonnull  some_bool\n");
    let der = GParamItem::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItem {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
            name: GParamName::from("some_bool"),
            markers: GMarkers(vec![GMarker::NonNull]),
        }
    );
}

#[test]
fn test_func_params_without_marker() {
    let driver = Reader::from("int my_var");
    let der = GParamItem::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItem {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
            name: GParamName::from("my_var"),
            markers: GMarkers(vec![]),
        }
    );

    let driver = Reader::from("bool \nsome_bool");
    let der = GParamItem::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItem {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
            name: GParamName::from("some_bool"),
            markers: GMarkers(vec![]),
        }
    );
}

#[test]
fn test_function_delceration() {
    let expected = GFunctionDecl {
        name: GFuncName::from("some_function"),
        params: vec![
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
                name: GParamName::from("some_int"),
                markers: GMarkers(vec![]),
            },
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
                name: GParamName::from("some_bool"),
                markers: GMarkers(vec![]),
            },
        ],
        return_value: GReturnValue {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Void)),
            markers: GMarkers(vec![]),
        },
        markers: GMarkers(vec![]),
    };

    let driver = Reader::from("void some_function(int some_int, bool some_bool);");
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);

    let driver = Reader::from("void some_function(int some_int ,bool some_bool);");
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);

    let driver = Reader::from("void some_function(int some_int,bool some_bool);");
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);

    let driver = Reader::from("void some_function(int some_int , bool some_bool);");
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);

    // Error (no comma)
    let driver = Reader::from("void some_function(int some_int bool some_bool);");
    let der = GFunctionDecl::derive(driver);
    assert!(der.is_err());
}

#[test]
fn test_function_delceration_with_markers() {
    let expected = GFunctionDecl {
        name: GFuncName::from("some_function"),
        params: vec![
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
                name: GParamName::from("some_int"),
                markers: GMarkers(vec![]),
            },
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
                name: GParamName::from("some_bool"),
                markers: GMarkers(vec![]),
            },
        ],
        return_value: GReturnValue {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Void)),
            markers: GMarkers(vec![]),
        },
        markers: GMarkers(vec![GMarker::TwExportStruct, GMarker::TWVisibilityDefault]),
    };

    let driver = Reader::from(
        "TW_EXPORT_STRUCT void some_function(int some_int, bool some_bool) TW_VISIBILITY_DEFAULT;",
    );
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);
}

#[test]
fn test_function_delceration_struct_return_value() {
    let expected = GFunctionDecl {
        name: GFuncName::from("some_function"),
        params: vec![
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
                name: GParamName::from("some_int"),
                markers: GMarkers(vec![]),
            },
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
                name: GParamName::from("some_bool"),
                markers: GMarkers(vec![]),
            },
        ],
        return_value: GReturnValue {
            ty: GType::Mutable(GTypeCategory::Pointer(Box::new(GTypeCategory::Struct(
                GStruct::from("SomeStruct"),
            )))),
            markers: GMarkers(vec![GMarker::Nullable]),
        },
        markers: GMarkers(vec![
            GMarker::TwExportStaticMethod,
            GMarker::TWVisibilityDefault,
        ]),
    };

    let driver = Reader::from(
        "TW_EXPORT_STATIC_METHOD struct SomeStruct* _Nullable some_function(int some_int, bool some_bool) TW_VISIBILITY_DEFAULT;",
    );
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);
}
