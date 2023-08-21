// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::{c_char, CStr, CString};

use ic_ledger_types::AccountIdentifier;
use k256::SecretKey;
use tw_memory::ffi::{
    c_byte_array::{CByteArray, CByteArrayResult},
    c_byte_array_ref::CByteArrayRef,
    c_result::ErrorCode,
};

use crate::{
    encode,
    sign::{self, Identity, SignTransferError},
    validation,
};

pub const KEY_SIZE_SECP256K1_BYTES: usize = 32;

#[repr(C)]
pub enum CEncodingCode {
    Ok = 0,
    InvalidPublicKey = 1,
    FailedDerEncode = 2,
    InvalidInput = 3,
}

impl From<encode::EncodePrincipalError> for CEncodingCode {
    fn from(error: encode::EncodePrincipalError) -> Self {
        match error {
            encode::EncodePrincipalError::InvalidPublicKey => CEncodingCode::InvalidPublicKey,
            encode::EncodePrincipalError::FailedDerEncode => CEncodingCode::FailedDerEncode,
        }
    }
}

impl From<CEncodingCode> for ErrorCode {
    fn from(error: CEncodingCode) -> Self {
        error as ErrorCode
    }
}

/// Determines if a provided string is a valid ICP address.
/// \param input *non-null* C-compatibile, nul-terminated string.
/// \return a boolean
#[no_mangle]
pub unsafe extern "C" fn tw_internetcomputer_is_address_valid(address: *const c_char) -> bool {
    let Ok(address) = CStr::from_ptr(address).to_str() else {
        return false;
    };

    if address.len() != 64 {
        return false;
    }

    validation::is_address_valid(address)
}

/// Encodes an expected secp256k1 extended public key to an Internet Computer principal.
/// \param pubkey *non-null* byte array.
/// \param pubkey_len the length of the `pubkey` byte array parameter.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn tw_internetcomputer_encode_public_key_to_principal(
    pubkey: *const u8,
    pubkey_len: usize,
) -> CByteArrayResult {
    let Some(slice) = CByteArrayRef::new(pubkey, pubkey_len).as_slice() else {
        return CByteArrayResult::error(0);
    };

    encode::encode_public_key_to_principal(slice)
        .map(CByteArray::from)
        .map_err(CEncodingCode::from)
        .into()
}

/// Encodes a principal byte array to an Internet Computer address.
/// \param principal_bytes *non-null* byte array.
/// \param principal_len the length of the `principal_bytes` array.
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn tw_internetcomputer_principal_to_account_identifer(
    principal_bytes: *const u8,
    principal_len: usize,
) -> *mut c_char {
    let data = std::slice::from_raw_parts(principal_bytes, principal_len);
    let address = encode::principal_to_account_identifier(data);
    CString::new(address)
        .expect("failed to make c-string")
        .into_raw()
}

/// Encodes a principal byte array to an Internet Computer principal text.
/// \param principal_bytes *non-null* byte array.
/// \param principal_len the length of the `principal_bytes` array.
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn tw_encode_textual_principal(
    principal_bytes: *const u8,
    principal_len: usize,
) -> *mut c_char {
    let data = std::slice::from_raw_parts(principal_bytes, principal_len);
    let textual_principal = encode::encode_textual_principal(data);
    CString::new(textual_principal)
        .expect("failed to make c-string")
        .into_raw()
}

#[repr(C)]
pub enum CSignTranserErrorCode {
    Ok = 0,
    InvalidPrivateKey = 1,
    FailedDerEncode = 2,
    InvalidToAccountIdentifier = 3,
    FailedEncodingSendArgs = 4,
    FailedEncodingSignedTransaction = 5,
    MalformedSignature = 6,
    FailedSignature = 7,
    InvalidAmount = 8,
}

impl From<SignTransferError> for CSignTranserErrorCode {
    fn from(value: SignTransferError) -> Self {
        match value {
            SignTransferError::Identity(error) => match error {
                sign::IdentityError::FailedPublicKeyDerEncoding => {
                    CSignTranserErrorCode::FailedDerEncode
                },
                sign::IdentityError::FailedSignature(_) => CSignTranserErrorCode::FailedSignature,
                sign::IdentityError::MalformedSignature => {
                    CSignTranserErrorCode::MalformedSignature
                },
            },
            SignTransferError::EncodingArgsFailed => CSignTranserErrorCode::FailedEncodingSendArgs,
            SignTransferError::EncodingSignedTransactionFailed => {
                CSignTranserErrorCode::FailedEncodingSignedTransaction
            },
        }
    }
}

impl From<CSignTranserErrorCode> for ErrorCode {
    fn from(value: CSignTranserErrorCode) -> Self {
        value as ErrorCode
    }
}

#[no_mangle]
pub unsafe extern "C" fn tw_internetcomputer_sign_transfer(
    privkey_bytes: *const u8,
    privkey_len: usize,
    to_account_identifier: *const c_char,
    amount: u64,
    memo: u64,
    current_timestamp_secs: u64,
) -> CByteArrayResult {
    if privkey_len != KEY_SIZE_SECP256K1_BYTES {
        return CByteArrayResult::error(CSignTranserErrorCode::InvalidPrivateKey);
    }
    let secret_key_bytes = std::slice::from_raw_parts(privkey_bytes, privkey_len);
    let Ok(secret_key) = SecretKey::from_slice(secret_key_bytes) else {
        return CByteArrayResult::error(CSignTranserErrorCode::InvalidPrivateKey);
    };
    let identity = match Identity::new(secret_key) {
        Ok(identity) => identity,
        Err(_) => return CByteArrayResult::error(CSignTranserErrorCode::FailedDerEncode),
    };
    if to_account_identifier.is_null() {
        return CByteArrayResult::error(CSignTranserErrorCode::InvalidToAccountIdentifier);
    }
    let Ok(textual_to_account_identifier) = CStr::from_ptr(to_account_identifier).to_str() else {
        return CByteArrayResult::error(CSignTranserErrorCode::InvalidToAccountIdentifier);
    };
    let Ok(to_account_identifier) =
        AccountIdentifier::from_hex(textual_to_account_identifier) else {
            return CByteArrayResult::error(CSignTranserErrorCode::InvalidToAccountIdentifier);
        };

    if amount == 0 {
        return CByteArrayResult::error(CSignTranserErrorCode::InvalidAmount);
    }

    sign::transfer(
        identity,
        to_account_identifier,
        amount as u64,
        memo,
        current_timestamp_secs,
    )
    .map(CByteArray::from)
    .map_err(CSignTranserErrorCode::from)
    .into()
}
