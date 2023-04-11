#[macro_use]
extern crate serde;

use grammar::{GHeaderFileItem, ParseTree};
use reader::Reader;

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

pub fn parse_file(path: &str) -> Result<Vec<GHeaderFileItem>> {
    let content = std::fs::read_to_string(path).map_err(|_err| Error::Todo)?;

    // TODO: Find a better solution for this.
    let content: String = content.chars().filter(|c| c.is_ascii()).collect();

    let mut reader = Reader::from(content.as_str());

    let mut items = vec![];

    loop {
        let (pending, checked_out) = reader.checkout();
        if let Ok(der) = GHeaderFileItem::derive(checked_out) {
            let (derived, branch) = (der.derived, der.branch);
            reader = pending.merge(branch);

            if let GHeaderFileItem::Eof = derived {
                break;
            }

            items.push(derived);
        } else {
            reader = pending.discard();
        }
    }

    Ok(items)
}

#[test]
#[ignore]
fn test_parse_file() {
    let res = parse_file("../include/TrustWalletCore/TWAnyAddress.h").unwrap();
    let string = serde_json::to_string_pretty(&res).unwrap();
    println!("{}", string);
}
