use crate::grammar::{GEof, GHeaderInclude, GNonAlphanumeric, GNonAlphanumericItem, ParseTree};
use crate::reader::Reader;

mod attributes;
mod functions;
mod keyword;
mod primitives;
mod separators;
mod type_categories;
mod typedef;

#[macro_export]
macro_rules! must_ok {
    ($ty:ty, $input:expr) => {{
        let res = <$ty>::derive(Reader::from($input));
        assert!(res.is_ok(), "{:?}", res);
    }};
    ($ty:ty, $input:expr, $expected:expr) => {{
        let res = <$ty>::derive(Reader::from($input));
        assert!(
            res.is_ok() && res.as_ref().unwrap().derived == $expected,
            "{:?} != {:?}",
            res,
            $expected
        );
    }};
}

#[macro_export]
macro_rules! must_err {
    ($ty:ty, $input:expr) => {{
        let res = <$ty>::derive(Reader::from($input));
        assert!(res.is_err());
    }};
}

#[test]
fn test_non_alphanumeric_items() {
    // OK!
    must_ok!(GNonAlphanumericItem, " ");
    must_ok!(GNonAlphanumericItem, ",");

    // ERR!
    must_err!(GNonAlphanumericItem, "a");
    must_err!(GNonAlphanumericItem, "1");
    must_err!(GNonAlphanumericItem, "-");
    must_err!(GNonAlphanumericItem, "_");
    // Does not handle EOF.
    must_err!(GNonAlphanumericItem, "");
}

#[test]
fn test_non_alphanumeric() {
    must_ok!(GNonAlphanumeric, ",,, ");
    must_ok!(GNonAlphanumeric, ",,,,");

    // ERR!
    must_err!(GNonAlphanumeric, ",,,a");
}

#[test]
fn test_eof() {
    must_ok!(GEof, "");
}
