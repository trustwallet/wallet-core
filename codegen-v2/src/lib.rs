// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#[macro_use]
extern crate serde;

use handlebars::{RenderError, TemplateError};
use serde_yaml::Error as YamlError;
use std::io::Error as IoError;

pub mod codegen;
pub mod manifest;
#[cfg(test)]
mod tests;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub enum Error {
    IoError(IoError),
    YamlError(YamlError),
    RenderError(RenderError),
    TemplateError(TemplateError),
    BadFormat(String),
    InvalidCommand,
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
