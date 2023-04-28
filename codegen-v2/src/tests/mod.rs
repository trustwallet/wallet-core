// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::grammar::{GEof, GNonAlphanumerics, GNonAlphanumericItem};

mod attributes;
mod comment;
mod enums;
mod functions;
mod keyword;
mod primitives;
mod separators;
mod structs;
mod type_categories;
mod typedef;

#[macro_export]
macro_rules! must_ok {
    ($ty:ty, $input:expr) => {{
        let res = <$ty as $crate::grammar::ParseTree>::derive($crate::reader::Reader::from($input));
        assert!(res.is_ok(), "{:?}", res);
    }};
    ($ty:ty, $input:expr, $expected:expr) => {{
        let res = <$ty as $crate::grammar::ParseTree>::derive($crate::reader::Reader::from($input));
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
        let res = <$ty as $crate::grammar::ParseTree>::derive($crate::reader::Reader::from($input));
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
    must_ok!(GNonAlphanumerics, ",,, ");
    must_ok!(GNonAlphanumerics, ",,,,");
    must_ok!(GNonAlphanumerics, ",,,a");
}

#[test]
fn test_eof() {
    must_ok!(GEof, "");
}
