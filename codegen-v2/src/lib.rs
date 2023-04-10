use grammar::{ParseTree, optional, wipe};
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
    let mut p_reader = Reader::from(file.as_str());

    loop {
        let (include_der, reader) = optional::<grammar::GHeaderInclude>(p_reader);
        if let Some(include_der) = include_der {
            dbg!(&include_der);

            // Ignore leading separators.
            let (der, reader) = wipe::<grammar::GSeparator>(reader);
            //dbg!(&der);

            p_reader = reader;
            continue;
        } else {
            p_reader = reader;
        }

        let (include_der, reader) = optional::<grammar::GAnyLine>(p_reader);
        if let Some(include_der) = include_der {
            // ...
        }

        p_reader = reader;

        let (include_der, reader) = optional::<grammar::GEof>(p_reader);
        if let Some(include_der) = include_der {
            break
        } else {
            p_reader = reader;
        }
    }
}

#[test]
fn test_parse_file() {
    parse_file("../include/TrustWalletCore/TWAnyAddress.h");
}
