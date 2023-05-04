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

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub enum Error {
    Todo,
    IoError(IoError),
    YamlError(YamlError),
    RenderError(RenderError),
    TemplateError(TemplateError),
    BadFormat(String),
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
