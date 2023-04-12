use crate::grammar::{GEof, GHeaderInclude, GNonAlphanumeric, GNonAlphanumericItem, ParseTree};
use crate::reader::Reader;
use std::fmt::Debug;

mod attributes;
mod functions;
mod primitives;
mod separators;
mod structs;
mod type_categories;
mod typedef;

fn must_ok<T>(input: &str, comp: Option<&T::Derivation>)
where
    T: ParseTree + Debug,
    <T as ParseTree>::Derivation: Debug + Eq + PartialEq,
{
    let reader = Reader::from(input);
    let res = T::derive(reader);

    dbg!(&res);

    if let Some(comp) = comp {
        let res = res.unwrap();
        assert_eq!(&res.derived, comp);
    } else {
        assert!(res.is_ok());
    }
}

fn must_err<T: ParseTree>(input: &str)
where
    T: ParseTree + Debug,
    <T as ParseTree>::Derivation: Debug,
{
    let driver = Reader::from(input);
    let res = T::derive(driver);

    dbg!(&res);
    assert!(res.is_err());
}

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
