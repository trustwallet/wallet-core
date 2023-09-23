// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::error::{SigningError, SigningErrorType};

pub mod decode;
pub mod function;
pub mod param;
pub mod param_token;
pub mod param_type;
pub mod prebuild;
pub mod token;

pub type AbiResult<T> = Result<T, AbiError>;

// TODO
#[derive(Debug)]
pub enum AbiError {
    InvalidParams,
    InvalidEncodedData,
    InvalidParamType,
    InvalidAddressValue,
    InvalidUintValue,
    MissingParamType,
    MissingParamValue,
    Internal,
}

impl From<ethabi::Error> for AbiError {
    fn from(_err: ethabi::Error) -> Self {
        AbiError::InvalidParams
    }
}

impl From<AbiError> for SigningError {
    fn from(_: AbiError) -> Self {
        SigningError(SigningErrorType::Error_internal)
    }
}
