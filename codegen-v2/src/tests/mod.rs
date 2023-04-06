use crate::grammar::{
    GEof, GFuncName, GFunctionDecl, GMarker, GNonAlphanumeric, GNonAlphanumericItem,
    GParamItemWithMarker, GParamItemWithoutMarker, GParamName, GSeparator, GSeparatorItem, GType,
    ParseTree,
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
fn test_types() {
    let driver = Reader::from("int");
    let der = GType::derive(driver).unwrap();
    assert_eq!(der.derived, GType::Int);

    let driver = Reader::from("bool");
    let der = GType::derive(driver).unwrap();
    assert_eq!(der.derived, GType::Bool);

    let driver = Reader::from("char\n");
    let der = GType::derive(driver).unwrap();
    assert_eq!(der.derived, GType::Char);
}

#[test]
fn test_func_params_with_marker() {
    let driver = Reader::from("int _Nonnull my_var\n");
    let der = GParamItemWithMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithMarker {
            ty: GType::Int,
            marker: GMarker::NonNull,
            name: GParamName::from("my_var".to_string())
        }
    );

    let driver = Reader::from("bool\n_Nonnull  some_bool\n");
    let der = GParamItemWithMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithMarker {
            ty: GType::Bool,
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
            ty: GType::Int,
            name: GParamName::from("my_var".to_string())
        }
    );

    let driver = Reader::from("bool \nsome_bool");
    let der = GParamItemWithoutMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithoutMarker {
            ty: GType::Bool,
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
                ty: GType::Int,
                name: GParamName::from("some_int".to_string()),
            },
            GParamItemWithoutMarker {
                ty: GType::Bool,
                name: GParamName::from("some_bool".to_string()),
            },
        ],
        return_ty: GType::Void,
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
