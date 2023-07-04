// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::any_signer::{AnySigner, SigningError};
use tw_memory::ffi::c_byte_array::{CByteArray, CByteArrayResult};
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::c_result::ErrorCode;

#[repr(C)]
pub enum CAnySignerError {
    Ok = 0,
    InvalidInput = 1,
    Unsupported = 2,
}

impl From<SigningError> for CAnySignerError {
    fn from(err: SigningError) -> Self {
        match err {
            SigningError::InvalidInput => CAnySignerError::InvalidInput,
            SigningError::Unsupported => CAnySignerError::Unsupported,
        }
    }
}

impl From<CAnySignerError> for ErrorCode {
    fn from(e: CAnySignerError) -> Self {
        e as ErrorCode
    }
}

/// Signs a transaction specified by the signing input and coin type.
///
/// \param input The serialized data of a signing input (e.g. TW.Bitcoin.Proto.SigningInput).
/// \param input_len The length of the signing input.
/// \param coin The given coin type to sign the transaction for.
/// \return The serialized data of a `SigningOutput` proto object. (e.g. TW.Bitcoin.Proto.SigningOutput).
#[no_mangle]
pub unsafe extern "C" fn tw_any_signer_sign(
    input: *const u8,
    input_len: usize,
    coin: u32,
) -> CByteArrayResult {
    let input_ref = CByteArrayRef::new(input, input_len);
    let Some(input) = input_ref.as_slice() else {
        return CByteArrayResult::error(CAnySignerError::InvalidInput);
    };

    AnySigner::sign(input, coin)
        .map(CByteArray::from)
        .map_err(CAnySignerError::from)
        .into()
}
