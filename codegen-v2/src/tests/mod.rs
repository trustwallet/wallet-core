use crate::{Driver, GSeparator, GType, ParseTree, GParamItemWithMarker};

#[test]
fn test_separator() {
    let driver = Driver::from("");
    let x = GSeparator::derive(driver).unwrap();
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
    dbg!(der.derived, GType::Int);

    let driver = Driver::from("bool ");
    let der = GType::derive(driver).unwrap();
    dbg!(der.derived, GType::Bool);

    let driver = Driver::from("char\n");
    let der = GType::derive(driver).unwrap();
    dbg!(der.derived, GType::Char);
}

#[test]
fn test_func_params_with_marker() {
    let driver = Driver::from("int _NOTNULL my_var");
    let der = GParamItemWithMarker::derive(driver).unwrap();
    dbg!(der);
}
