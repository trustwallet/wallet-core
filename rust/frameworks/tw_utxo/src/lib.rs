// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Utxo::Proto;

pub mod address;
pub mod compiler;
pub mod encode;
pub mod error;
pub mod planner;
pub mod script;
pub mod sighash;
pub mod sighash_computer;
pub mod signing_mode;
pub mod transaction;
pub mod utxo_selector;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub struct Error(Proto::Error);

impl From<Proto::Error> for Error {
    fn from(value: Proto::Error) -> Self {
        Error(value)
    }
}

impl From<bitcoin::sighash::Error> for Error {
    fn from(_value: bitcoin::sighash::Error) -> Self {
        Error(Proto::Error::Error_sighash_failed)
    }
}

impl From<Error> for Proto::Error {
    fn from(value: Error) -> Self {
        value.0
    }
}
