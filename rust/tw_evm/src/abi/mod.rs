// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::Address;
use tw_coin_entry::error::{SigningError, SigningErrorType};
use tw_number::U256;

pub mod prebuild;

pub type AbiResult<T> = Result<T, AbiError>;

#[derive(Debug)]
pub enum AbiError {
    InvalidParams,
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

/// TODO refactor this by implementing `RlpEncode` for `U256` at the next iteration.
pub fn convert_u256(num: U256) -> ethabi::Uint {
    let bytes = num.to_big_endian().take();
    ethabi::Uint::from_big_endian(&bytes)
}

/// TODO refactor this by implementing `RlpEncode` for `Address` at the next iteration.
pub fn convert_address(addr: Address) -> ethabi::Address {
    ethabi::Address::from(addr.bytes().take())
}
