use crate::grammar::{
    GEof, GFuncName, GFunctionDecl, GMarker, GNonAlphanumeric, GNonAlphanumericItem,
    GParamItemWithMarker, GParamItemWithoutMarker, GParamName, GPrimitive, GSeparator,
    GSeparatorItem, GStruct, GTypeCategory, ParseTree,
};
use crate::reader::Reader;

#[test]
fn test_non_alphanumeric_items() {
    let driver = Reader::from("a");
    let res = GNonAlphanumericItem::derive(driver);
    assert!(res.is_err());

    let driver = Reader::from("1");
    let res = GNonAlphanumericItem::derive(driver);
    assert!(res.is_err());

    let driver = Reader::from("-");
    let res = GNonAlphanumericItem::derive(driver);
    assert!(res.is_err());

    let driver = Reader::from("_");
    let res = GNonAlphanumericItem::derive(driver);
    assert!(res.is_err());

    // Does not handle EOF.
    let driver = Reader::from("");
    let res = GNonAlphanumericItem::derive(driver);
    assert!(res.is_err());

    // OK!
    let driver = Reader::from(" ");
    let res = GNonAlphanumericItem::derive(driver);
    assert!(res.is_ok());

    // OK!
    let driver = Reader::from(",");
    let res = GNonAlphanumericItem::derive(driver);
    assert!(res.is_ok());
}

#[test]
fn test_non_alphanumeric() {
    let driver = Reader::from(",,, ");
    let res = GNonAlphanumeric::derive(driver);
    assert!(res.is_ok());

    let driver = Reader::from(",,,,");
    let res = GNonAlphanumeric::derive(driver);
    assert!(res.is_ok());
}

#[test]
fn test_eof() {
    let driver = Reader::from("");
    let res = GEof::derive(driver).unwrap();
    assert_eq!(res.derived, GEof);
}

#[test]
fn test_separator_items() {
    let driver = Reader::from(" ");
    let res = GSeparatorItem::derive(driver).unwrap();
    assert_eq!(res.derived, GSeparatorItem::Space);

    let driver = Reader::from("\n");
    let res = GSeparatorItem::derive(driver).unwrap();
    assert_eq!(res.derived, GSeparatorItem::Newline);

    let driver = Reader::from("\t");
    let res = GSeparatorItem::derive(driver).unwrap();
    assert_eq!(res.derived, GSeparatorItem::Tab);
}

#[test]
fn test_separator() {
    let driver = Reader::from(" ");
    let x = GSeparator::derive(driver).unwrap();
    dbg!(x);

    let driver = Reader::from("  ");
    let x = GSeparator::derive(driver).unwrap();
    dbg!(x);

    let driver = Reader::from(" \n  \t");
    let x = GSeparator::derive(driver).unwrap();
    dbg!(x);
}

#[test]
fn test_types_categories() {
    let driver = Reader::from("int");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(der.derived, GTypeCategory::Scalar(GPrimitive::Int));

    let driver = Reader::from("int*");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(GPrimitive::Int)))
    );

    let driver = Reader::from("int **");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Scalar(GPrimitive::Int)
        ))))
    );

    let driver = Reader::from("int * * *");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(GPrimitive::Int)))
        ))))
    );

    let driver = Reader::from("Unknown");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(der.derived, GTypeCategory::Unknown("Unknown".to_string()));

    let driver = Reader::from("Unknown **");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Unknown("Unknown".to_string())
        ))))
    );

    let driver = Reader::from("Unknown * * *");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Unknown("Unknown".to_string())))
        ))))
    );
}

#[test]
fn test_types() {
    let driver = Reader::from("int");
    let der = GPrimitive::derive(driver).unwrap();
    assert_eq!(der.derived, GPrimitive::Int);

    let driver = Reader::from("bool");
    let der = GPrimitive::derive(driver).unwrap();
    assert_eq!(der.derived, GPrimitive::Bool);

    let driver = Reader::from("char\n");
    let der = GPrimitive::derive(driver).unwrap();
    assert_eq!(der.derived, GPrimitive::Char);
}

#[test]
fn test_func_params_with_marker() {
    let driver = Reader::from("int _Nonnull my_var\n");
    let der = GParamItemWithMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithMarker {
            ty: GPrimitive::Int,
            marker: GMarker::NonNull,
            name: GParamName::from("my_var".to_string())
        }
    );

    let driver = Reader::from("bool\n_Nonnull  some_bool\n");
    let der = GParamItemWithMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithMarker {
            ty: GPrimitive::Bool,
            marker: GMarker::NonNull,
            name: GParamName::from("some_bool".to_string())
        }
    );
}

#[test]
fn test_func_params_without_marker() {
    let driver = Reader::from("int my_var");
    let der = GParamItemWithoutMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithoutMarker {
            ty: GPrimitive::Int,
            name: GParamName::from("my_var".to_string())
        }
    );

    let driver = Reader::from("bool \nsome_bool");
    let der = GParamItemWithoutMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithoutMarker {
            ty: GPrimitive::Bool,
            name: GParamName::from("some_bool".to_string())
        }
    );
}

#[test]
fn test_function_delceration() {
    let expected = GFunctionDecl {
        name: GFuncName::from("some_function".to_string()),
        params: vec![
            GParamItemWithoutMarker {
                ty: GPrimitive::Int,
                name: GParamName::from("some_int".to_string()),
            },
            GParamItemWithoutMarker {
                ty: GPrimitive::Bool,
                name: GParamName::from("some_bool".to_string()),
            },
        ],
        return_ty: GPrimitive::Void,
        markers: vec![],
    };

    let driver = Reader::from("void some_function(int some_int, bool some_bool)");
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);

    let driver = Reader::from("void some_function(int some_int ,bool some_bool)");
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);

    let driver = Reader::from("void some_function(int some_int,bool some_bool)");
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);

    let driver = Reader::from("void some_function(int some_int , bool some_bool)");
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);

    // Error (no comma)
    let driver = Reader::from("void some_function(int some_int bool some_bool)");
    let der = GFunctionDecl::derive(driver);
    assert!(der.is_err());
}

#[test]
fn test_function_delceration_with_markers() {
    let expected = GFunctionDecl {
        name: GFuncName::from("some_function".to_string()),
        params: vec![
            GParamItemWithoutMarker {
                ty: GPrimitive::Int,
                name: GParamName::from("some_int".to_string()),
            },
            GParamItemWithoutMarker {
                ty: GPrimitive::Bool,
                name: GParamName::from("some_bool".to_string()),
            },
        ],
        return_ty: GPrimitive::Void,
        markers: vec![GMarker::TwExportStruct, GMarker::TWVisibilityDefault],
    };

    let driver = Reader::from(
        "TW_EXPORT_STRUCT void some_function(int some_int, bool some_bool) TW_VISIBILITY_DEFAULT",
    );
    let der = GFunctionDecl::derive(driver).unwrap();
    assert_eq!(der.derived, expected);
}
