// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#[macro_use]
extern crate serde;

use handlebars::{RenderError, TemplateError};
use serde_yaml::Error as YamlError;
use std::io;
use std::io::Error as IoError;
use toml_edit::TomlError;

pub mod codegen;
pub mod coin_id;
pub mod manifest;
pub mod registry;
#[cfg(test)]
mod tests;
pub mod utils;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub enum Error {
    IoError(IoError),
    YamlError(YamlError),
    RenderError(RenderError),
    TemplateError(TemplateError),
    BadFormat(String),
    RegistryError(String),
    TomlFormat(String),
    InvalidCommand,
}

impl Error {
    pub fn io_error_other(err: String) -> Error {
        Error::IoError(IoError::new(io::ErrorKind::Other, err))
    }
}

impl From<IoError> for Error {
    fn from(err: IoError) -> Self {
        Error::IoError(err)
    }
}

impl From<YamlError> for Error {
    fn from(err: YamlError) -> Self {
        Error::YamlError(err)
    }
}

impl From<RenderError> for Error {
    fn from(err: RenderError) -> Self {
        Error::RenderError(err)
    }
}

impl From<TemplateError> for Error {
    fn from(err: TemplateError) -> Self {
        Error::TemplateError(err)
    }
}

impl From<TomlError> for Error {
    fn from(err: TomlError) -> Self {
        Error::TomlFormat(err.to_string())
    }
}

fn current_year() -> u64 {
    use std::time::{SystemTime, UNIX_EPOCH};

    let now = SystemTime::now();
    let seconds_since_epoch = now
        .duration_since(UNIX_EPOCH)
        .expect("System's time is set before the start of the Unix epoch");

    // One Gregorian calendar year has 365.2425 days,
    // respectively 31556952 seconds.
    1970 + (seconds_since_epoch.as_secs() / 31556952)
}
