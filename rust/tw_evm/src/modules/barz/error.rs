// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::AddressError;
use tw_encoding::hex::FromHexError;
use tw_keypair::KeyPairError;
use tw_number::NumberError;

use crate::{abi::AbiError, message::MessageSigningError};

pub type BarzResult<T> = Result<T, BarzError>;

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub enum BarzError {
    InvalidInput,
    AddressError,
    FromHexError,
    AbiError,
    NumberError,
    KeypairError,
    HashError,
    MessageSigningError,
    SerializationError,
}

impl From<AddressError> for BarzError {
    fn from(_: AddressError) -> Self {
        BarzError::AddressError
    }
}

impl From<FromHexError> for BarzError {
    fn from(_: FromHexError) -> Self {
        BarzError::FromHexError
    }
}

impl From<AbiError> for BarzError {
    fn from(_: AbiError) -> Self {
        BarzError::AbiError
    }
}

impl From<NumberError> for BarzError {
    fn from(_: NumberError) -> Self {
        BarzError::NumberError
    }
}

impl From<KeyPairError> for BarzError {
    fn from(_: KeyPairError) -> Self {
        BarzError::KeypairError
    }
}

impl From<tw_hash::Error> for BarzError {
    fn from(_: tw_hash::Error) -> Self {
        BarzError::HashError
    }
}

impl From<MessageSigningError> for BarzError {
    fn from(_: MessageSigningError) -> Self {
        BarzError::MessageSigningError
    }
}

impl From<serde_json::Error> for BarzError {
    fn from(_: serde_json::Error) -> Self {
        BarzError::SerializationError
    }
}
