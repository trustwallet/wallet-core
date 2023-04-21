// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::grammar::{GSeparators, GSeparatorItem, ParseTree};
use crate::must_ok;
use crate::reader::Reader;

#[test]
fn test_separator_items() {
    must_ok!(GSeparatorItem, " ", GSeparatorItem::Space);
    must_ok!(GSeparatorItem, "\n", GSeparatorItem::Newline);
    must_ok!(GSeparatorItem, "\t", GSeparatorItem::Tab);
}

#[test]
// TODO: Assert
fn test_separator() {
    let driver = Reader::from(" ");
    let x = GSeparators::derive(driver).unwrap();
    dbg!(x);

    let driver = Reader::from("  ");
    let x = GSeparators::derive(driver).unwrap();
    dbg!(x);

    let driver = Reader::from(" \n  \t");
    let x = GSeparators::derive(driver).unwrap();
    dbg!(x);
}
