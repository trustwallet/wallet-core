// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{Error, Result};
use std::fs;
use std::path::PathBuf;
use std::str::FromStr;

const NEW_LINE_TAB_DECORATOR: &str = "\n    ";
const NO_DECORATOR: &str = "";

pub struct Workspace {
    path_to_toml: PathBuf,
}

impl Workspace {
    pub fn new(path_to_toml: PathBuf) -> Workspace {
        Workspace { path_to_toml }
    }

    pub fn insert_crate(self, path_to_crate: PathBuf) -> Result<()> {
        let manifest = fs::read_to_string(&self.path_to_toml)?;
        let mut manifest = toml_edit::Document::from_str(&manifest)?;

        let members = manifest["workspace"]["members"]
            .as_array_mut()
            .ok_or(Error::TomlFormat(
                "Invalid 'workspace' TOML format".to_string(),
            ))?;

        // Try to get a `path_to_crate` relative to the `path_to_toml`
        let absolute_path_to_manifest = self.path_to_toml.parent().unwrap().canonicalize()?;
        let relative_path_to_crate = path_to_crate
            .canonicalize()?
            .strip_prefix(absolute_path_to_manifest)
            .map_err(|e| Error::io_error_other(e.to_string()))?
            .to_str()
            .ok_or_else(|| Error::io_error_other("Invalid path to the crate".to_string()))?
            .to_string();

        // Push the new member and saves sort the manifest.

        let relative_path_to_crate_decorated =
            toml_edit::Value::from(relative_path_to_crate.to_string())
                .decorated(NEW_LINE_TAB_DECORATOR, NO_DECORATOR);

        members.push_formatted(relative_path_to_crate_decorated);
        members.sort_by(|x, y| x.as_str().cmp(&y.as_str()));

        fs::write(self.path_to_toml, manifest.to_string())?;
        Ok(())
    }
}
