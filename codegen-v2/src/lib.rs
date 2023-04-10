use grammar::ParseTree;
use reader::{Reader, ReaderBranch};

mod grammar;
mod reader;
#[cfg(test)]
mod tests;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub enum Error {
    Todo,
    Eof,
}

pub fn parse_file(path: &str) {
    let file = std::fs::read_to_string(path).unwrap();
    let mut reader = Reader::from(file.as_str());

    loop {
        let (pending, checked_out) = reader.checkout();
        if let Ok(derived) = grammar::GHeaderInclude::derive(checked_out) {
            dbg!(&derived);
            reader = pending.merge(derived.branch);
        } else {
            reader = pending.discard();
        }

        let (pending, checked_out) = reader.checkout();
        if let Ok(derived) = grammar::GAnyLine::derive(checked_out) {
            //dbg!(&derived);
            reader = pending.merge(derived.branch);
        } else {
            reader = pending.discard();
        }

        let (pending, checked_out) = reader.checkout();
        if grammar::GEof::derive(checked_out).is_ok() {
            break;
        } else {
            reader = pending.discard();
        }
    }
}

#[test]
fn test_parse_file() {
    parse_file("../include/TrustWalletCore/TWAnyAddress.h");
}
