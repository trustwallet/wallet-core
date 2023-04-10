use crate::grammar::{
    GEof, GFuncName, GFunctionDecl, GHeaderInclude, GMarker, GNonAlphanumeric,
    GNonAlphanumericItem, GParamItem, GParamName, GPrimitive, GSeparator, GSeparatorItem, GStruct,
    GTypeCategory, ParseTree,
};
use crate::reader::Reader;

#[test]
fn test_func_params_with_marker() {
    let driver = Reader::from("int _Nonnull my_var\n");
    let der = GParamItem::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItem {
            ty: GPrimitive::Int,
            name: GParamName::from("my_var".to_string()),
            markers: vec![GMarker::NonNull],
        }
    );

    let driver = Reader::from("bool\n_Nonnull  some_bool\n");
    let der = GParamItem::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItem {
            ty: GPrimitive::Bool,
            name: GParamName::from("some_bool".to_string()),
            markers: vec![GMarker::NonNull],
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
            ty: GPrimitive::Int,
            name: GParamName::from("my_var".to_string()),
            markers: vec![],
        }
    );

    let driver = Reader::from("bool \nsome_bool");
    let der = GParamItem::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItem {
            ty: GPrimitive::Bool,
            name: GParamName::from("some_bool".to_string()),
            markers: vec![],
        }
    );
}

#[test]
fn test_function_delceration() {
    let expected = GFunctionDecl {
        name: GFuncName::from("some_function".to_string()),
        params: vec![
            GParamItem {
                ty: GPrimitive::Int,
                name: GParamName::from("some_int".to_string()),
                markers: vec![],
            },
            GParamItem {
                ty: GPrimitive::Bool,
                name: GParamName::from("some_bool".to_string()),
                markers: vec![],
            },
        ],
        return_ty: GPrimitive::Void,
        markers: vec![],
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
        name: GFuncName::from("some_function".to_string()),
        params: vec![
            GParamItem {
                ty: GPrimitive::Int,
                name: GParamName::from("some_int".to_string()),
                markers: vec![],
            },
            GParamItem {
                ty: GPrimitive::Bool,
                name: GParamName::from("some_bool".to_string()),
                markers: vec![],
            },
        ],
        return_ty: GPrimitive::Void,
        markers: vec![GMarker::TwExportStruct, GMarker::TWVisibilityDefault],
    };

    let driver = Reader::from(
        "TW_EXPORT_STRUCT void some_function(int some_int, bool some_bool) TW_VISIBILITY_DEFAULT;",
    );
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);
}
