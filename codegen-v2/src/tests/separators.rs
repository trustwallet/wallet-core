use crate::grammar::{GSeparator, GSeparatorItem, ParseTree};
use crate::reader::Reader;
use crate::must_ok;

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
    let x = GSeparator::derive(driver).unwrap();
    dbg!(x);

    let driver = Reader::from("  ");
    let x = GSeparator::derive(driver).unwrap();
    dbg!(x);

    let driver = Reader::from(" \n  \t");
    let x = GSeparator::derive(driver).unwrap();
    dbg!(x);
}
