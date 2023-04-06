use crate::grammar::{
    GEof, GFuncParams, GMarker, GParamItemWithMarker, GParamItemWithoutMarker, GParamName,
    GSeparator, GSeparatorItem, GType, ParseTree,
};
use crate::reader::Reader;

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
    let driver = Reader::from("int _NOTNULL my_var\n");
    let der = GParamItemWithMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithMarker {
            ty: GType::Int,
            marker: GMarker::from("_NOTNULL".to_string()),
            name: GParamName::from("my_var".to_string())
        }
    );

    let driver = Reader::from("bool\n_SOMEMARKER  some_bool\n");
    let der = GParamItemWithMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithMarker {
            ty: GType::Bool,
            marker: GMarker::from("_SOMEMARKER".to_string()),
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
fn test_func_params_multiple() {
    let driver = Reader::from("int my_int , bool my_bool");
    let der = GFuncParams::derive(driver).unwrap();
    dbg!(der);
}
