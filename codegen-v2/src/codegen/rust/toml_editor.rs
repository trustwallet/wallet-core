// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{Error, Result};
use std::fs;
use std::path::{Path, PathBuf};
use std::str::FromStr;
use toml_edit::{Document, InlineTable, Item, Value};

const NEW_LINE_TAB_DECORATOR: &str = "\n    ";
const NO_DECORATOR: &str = "";

pub struct Workspace {
    path_to_toml: PathBuf,
}

impl Workspace {
    pub fn new(path_to_toml: PathBuf) -> Workspace {
        Workspace { path_to_toml }
    }

    pub fn insert_crate(self, path_to_crate: &Path) -> Result<()> {
        let manifest = fs::read_to_string(&self.path_to_toml)?;
        let mut manifest = Document::from_str(&manifest)?;

        let members = manifest["workspace"]["members"]
            .as_array_mut()
            .ok_or(Error::TomlFormat(
                "Invalid 'workspace' TOML format".to_string(),
            ))?;

        // Try to get a path to the crate relative to the `Cargo.toml`.
        let relative_path_to_crate = relative_path_to_crate(&self.path_to_toml, path_to_crate)?;

        // Push the new member, sort and save the manifest.

        let relative_path_to_crate_decorated =
            Value::from(relative_path_to_crate).decorated(NEW_LINE_TAB_DECORATOR, NO_DECORATOR);

        members.push_formatted(relative_path_to_crate_decorated);
        members.sort_by(|x, y| x.as_str().cmp(&y.as_str()));

        fs::write(self.path_to_toml, manifest.to_string())?;
        Ok(())
    }
}

pub struct Dependencies {
    path_to_toml: PathBuf,
}

impl Dependencies {
    pub fn new(path_to_toml: PathBuf) -> Dependencies {
        Dependencies { path_to_toml }
    }

    pub fn insert_dependency(self, dep_name: &str, path_to_dep_crate: &Path) -> Result<()> {
        let manifest = fs::read_to_string(&self.path_to_toml)?;
        let mut manifest = Document::from_str(&manifest)?;

        let dependencies = manifest["dependencies"]
            .as_table_like_mut()
            .ok_or(Error::TomlFormat("Invalid 'Cargo.toml' format".to_string()))?;

        // Try to get a path to the crate relative to the `Cargo.toml`.
        let relative_path_to_crate = relative_path_to_crate(&self.path_to_toml, path_to_dep_crate)?;

        // Create the new dependency member (aka a TOML inline table with `path` key-value).
        let mut new_member = InlineTable::new();
        new_member.insert("path", relative_path_to_crate.into());

        // Push the new member, sort and save the manifest.
        dependencies.insert(dep_name, Item::Value(Value::InlineTable(new_member)));
        dependencies.sort_values();

        fs::write(self.path_to_toml, manifest.to_string())?;

        Ok(())
    }
}

/// Returns a path to the dependency accordingly to the Cargo manifest file.
/// The result string can be put to `Cargo.toml` as:
/// ```toml
/// tw_foo = { path = "<RESULT_PATH>" }
/// ```
fn relative_path_to_crate(
    path_to_cargo_manifest: &Path,
    path_to_dependency: &Path,
) -> Result<String> {
    let absolute_path_to_crate_directory = path_to_cargo_manifest
        .parent()
        .ok_or_else(|| Error::io_error_other("Cannot get a parent directory".to_string()))?
        .canonicalize()?;
    let absolute_path_to_dependency = path_to_dependency.canonicalize()?;

    let relative_path_to_dependency = pathdiff::diff_paths(
        absolute_path_to_dependency,
        absolute_path_to_crate_directory,
    )
    .ok_or_else(|| {
        Error::io_error_other("Cannot get a relative path to the dependency".to_string())
    })?
    .to_str()
    .ok_or_else(|| Error::io_error_other("Invalid path to the crate".to_string()))?
    .to_string();

    Ok(relative_path_to_dependency)
}
