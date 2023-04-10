use grammar::{GHeaderFileItem, ParseTree};
use reader::{Reader};

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
        if let Ok(der) = GHeaderFileItem::derive(checked_out) {
            let (derived, branch) = (der.derived, der.branch);
            reader = pending.merge(branch);

            if let GHeaderFileItem::Unrecognized(der) = &derived {
                // ...
                dbg!(&der);
            } else {
                //dbg!(&derived);
            }

            if let GHeaderFileItem::Eof = derived {
                break;
            }
        } else {
            reader = pending.discard();
        }
    }
}

#[test]
fn test_parse_file() {
    parse_file("../include/TrustWalletCore/TWAnyAddress.h");
}
