use crate::{
    Driver, GEof, GMarker, GParamItemWithMarker, GParamItemWithoutMarker, GParamName, GSeparator,
    GType, ParseTree, GSeparatorItem,
};

#[test]
fn test_separator_items() {
    let driver = Driver::from(" ");
    let res = GSeparatorItem::derive(driver).unwrap();
    assert_eq!(res.derived, GSeparatorItem::Space);

    let driver = Driver::from("\n");
    let res = GSeparatorItem::derive(driver).unwrap();
    assert_eq!(res.derived, GSeparatorItem::Newline);

    let driver = Driver::from("\t");
    let res = GSeparatorItem::derive(driver).unwrap();
    assert_eq!(res.derived, GSeparatorItem::Tab);
}

#[test]
fn test_separator() {
    let driver = Driver::from("");
    let x = GEof::derive(driver).unwrap();
    dbg!(x);

    let driver = Driver::from(" ");
    let x = GSeparator::derive(driver).unwrap();
    dbg!(x);

    let driver = Driver::from("  ");
    let x = GSeparator::derive(driver).unwrap();
    dbg!(x);

    let driver = Driver::from(" \n  \t");
    let x = GSeparator::derive(driver).unwrap();
    dbg!(x);
}

#[test]
fn test_types() {
    let driver = Driver::from("int");
    let der = GType::derive(driver).unwrap();
    assert_eq!(der.derived, GType::Int);

    let driver = Driver::from("bool");
    let der = GType::derive(driver).unwrap();
    assert_eq!(der.derived, GType::Bool);

    let driver = Driver::from("char\n");
    let der = GType::derive(driver).unwrap();
    assert_eq!(der.derived, GType::Char);
}

#[test]
fn test_func_params_with_marker() {
    let driver = Driver::from("int _NOTNULL my_var\n");
    let der = GParamItemWithMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithMarker {
            ty: GType::Int,
            marker: GMarker("_NOTNULL".to_string()),
            name: GParamName("my_var".to_string())
        }
    );

    let driver = Driver::from("bool\n_SOMEMARKER  some_bool\n");
    let der = GParamItemWithMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithMarker {
            ty: GType::Bool,
            marker: GMarker("_SOMEMARKER".to_string()),
            name: GParamName("some_bool".to_string())
        }
    );
}

#[test]
fn test_func_params_without_marker() {
    let driver = Driver::from("int my_var");
    let der = GParamItemWithoutMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithoutMarker {
            ty: GType::Int,
            name: GParamName("my_var".to_string())
        }
    );

    let driver = Driver::from("bool \nsome_bool");
    let der = GParamItemWithoutMarker::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GParamItemWithoutMarker {
            ty: GType::Bool,
            name: GParamName("some_bool".to_string())
        }
    );
}
