use crate::grammar::{GEof, GHeaderInclude, GNonAlphanumeric, GNonAlphanumericItem, ParseTree};
use crate::reader::Reader;

mod attributes;
mod functions;
mod primitives;
mod separators;
mod structs;
mod type_categories;
mod typedef;

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

    // ERR!
    let driver = Reader::from(",,,,a");
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
fn test_header_include() {
    let driver = Reader::from("#include \"some_file.h\"");
    let res = GHeaderInclude::derive(driver).unwrap();
    dbg!(res);
}
