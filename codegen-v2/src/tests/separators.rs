use crate::grammar::{
    GEof, GFuncName, GFunctionDecl, GHeaderInclude, GMarker, GNonAlphanumeric,
    GNonAlphanumericItem, GParamItem, GParamName, GPrimitive, GSeparator, GSeparatorItem, GStruct,
    GTypeCategory, ParseTree,
};
use crate::reader::Reader;

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
