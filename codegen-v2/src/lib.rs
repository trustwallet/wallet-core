// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#[macro_use]
extern crate serde;

use grammar::{GHeaderFileItem, ParseTree};
use reader::Reader;
use std::{
    collections::HashMap,
    path::{Path, PathBuf},
};

mod grammar;
mod reader;
#[cfg(test)]
mod tests;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub enum Error {
    Todo,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct CHeaderDirectory {
    pub map: HashMap<PathBuf, Vec<GHeaderFileItem>>,
}

pub fn parse(path: &Path) -> Result<CHeaderDirectory> {
    let dir = CHeaderDirectory {
        map: HashMap::new(),
    };

    let dir = parse_dir(path, dir)?;
    Ok(dir)
}

fn parse_dir(path: &Path, mut main_dir: CHeaderDirectory) -> Result<CHeaderDirectory> {
    let entries = std::fs::read_dir(path).map_err(|_| Error::Todo)?;
    for entry in entries {
        let child_path = entry.map_err(|_| Error::Todo)?.path();

        if child_path.is_dir() {
            main_dir = parse_dir(&child_path, main_dir)?;
        } else if child_path.is_file() {
            main_dir = parse_file(&child_path, main_dir)?;
        } else {
            panic!()
        }
    }

    Ok(main_dir)
}

fn parse_file(path: &Path, mut main_dir: CHeaderDirectory) -> Result<CHeaderDirectory> {
    if !path.is_file() {
        return Err(Error::Todo);
    }

    let parent = path.parent().ok_or(Error::Todo)?;

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

            if let GHeaderFileItem::HeaderInclude(include) = &derived {
                let child_path = parent.join(Path::new(&include.0));

                // Skip self-referencing includes.
                if child_path == path {
                    continue;
                }

                // If the `#include`d file is not yet parsed, parse it.
                // Otherwise, skip it.
                if !main_dir.map.contains_key(&child_path) {
                    main_dir = parse_file(&child_path, main_dir)?;
                }
            }

            // Indicates EOF, implying this loop must exit.
            if let GHeaderFileItem::Eof = derived {
                break;
            }

            items.push(derived);
        } else {
            reader = pending.discard();
        }
    }

    // Update the map with newly parsed items.
    main_dir.map.insert(path.to_path_buf(), items);

    Ok(main_dir)
}

#[test]
#[ignore]
fn test_parse_with_output() {
    let path = Path::new("../include/");
    let dir = parse(&path).unwrap();

    println!("UNRECOGNIZED items:");
    for (path, items) in dir.map {
        println!("> ## FILE: {}", path.to_str().unwrap());
        for item in items {
            if let GHeaderFileItem::Unrecognized(item) = item {
                println!("  > {}", item);
            }
        }
    }
}
