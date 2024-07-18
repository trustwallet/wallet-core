// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::{base32, base58, base64, hex, EncodingError};
use std::ffi::{c_char, CStr, CString};
use tw_memory::ffi::c_byte_array::{CByteArray, CByteArrayResult};
use tw_memory::ffi::c_result::{CStrMutResult, ErrorCode};

#[repr(C)]
pub enum CEncodingCode {
    Ok = 0,
    InvalidInput = 1,
    InvalidAlphabet = 2,
    Internal = 3,
}

impl From<EncodingError> for CEncodingCode {
    fn from(error: EncodingError) -> Self {
        match error {
            EncodingError::InvalidInput => CEncodingCode::InvalidInput,
            EncodingError::InvalidAlphabet => CEncodingCode::InvalidAlphabet,
            EncodingError::Internal => CEncodingCode::Internal,
        }
    }
}

impl From<hex::FromHexError> for CEncodingCode {
    fn from(_error: hex::FromHexError) -> Self {
        CEncodingCode::InvalidInput
    }
}

impl From<CEncodingCode> for ErrorCode {
    fn from(error: CEncodingCode) -> Self {
        error as ErrorCode
    }
}

#[repr(C)]
#[derive(Debug, PartialEq)]
#[cfg_attr(feature = "arbitrary", derive(arbitrary::Arbitrary))]
pub enum Base58Alphabet {
    Bitcoin = 1,
    Ripple = 2,
}

impl From<Base58Alphabet> for base58::Alphabet {
    fn from(value: Base58Alphabet) -> Self {
        match value {
            Base58Alphabet::Bitcoin => base58::Alphabet::Bitcoin,
            Base58Alphabet::Ripple => base58::Alphabet::Ripple,
        }
    }
}

/// Encodes the `input` data as base32.
/// \param input *non-null* byte array.
/// \param alphabet *optional* C-compatible, nul-terminated string.
///                `ALPHABET_RFC4648` is used by default if `alphabet` is null.
/// \param padding whether the padding bytes should be included.
/// \return C-compatible result with a C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn encode_base32(
    input: *const u8,
    input_len: usize,
    alphabet: *const c_char,
    padding: bool,
) -> CStrMutResult {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };

    let alphabet = match get_alphabet(alphabet) {
        Ok(alphabet) => alphabet,
        Err(e) => return CStrMutResult::error(e),
    };

    base32::encode(input, alphabet, padding)
        .map(|result| CString::new(result).unwrap().into_raw())
        .map_err(CEncodingCode::from)
        .into()
}

/// Decodes the base32 `input` string.
/// \param input *non-null* C-compatible, nul-terminated string.
/// \param alphabet *optional* C-compatible, nul-terminated string.
///                `ALPHABET_RFC4648` is used by default if `alphabet` is null.
/// \param padding whether the padding bytes should be trimmed when decoding.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn decode_base32(
    input: *const c_char,
    alphabet: *const c_char,
    padding: bool,
) -> CByteArrayResult {
    let input = match CStr::from_ptr(input).to_str() {
        Ok(input) => input,
        Err(_) => return CByteArrayResult::error(CEncodingCode::InvalidInput),
    };
    let alphabet = match get_alphabet(alphabet) {
        Ok(alphabet) => alphabet,
        Err(e) => return CByteArrayResult::error(e),
    };

    base32::decode(input, alphabet, padding)
        .map(CByteArray::from)
        .map_err(CEncodingCode::from)
        .into()
}

/// Encodes the `input` data as base58.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \param alphabet alphabet type.
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn encode_base58(
    input: *const u8,
    input_len: usize,
    alphabet: Base58Alphabet,
) -> *mut c_char {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    CString::new(base58::encode(input, alphabet.into()))
        .unwrap()
        .into_raw()
}

/// Decodes the base58 `input` string.
/// \param input *non-null* C-compatible, nul-terminated string.
/// \param alphabet alphabet type.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn decode_base58(
    input: *const c_char,
    alphabet: Base58Alphabet,
) -> CByteArrayResult {
    let input = match CStr::from_ptr(input).to_str() {
        Ok(input) => input,
        Err(_) => return CByteArrayResult::error(CEncodingCode::InvalidInput),
    };

    base58::decode(input, alphabet.into())
        .map(CByteArray::from)
        .map_err(CEncodingCode::from)
        .into()
}

/// Encodes the `data` data as a padded, base64 string.
/// \param data *non-null* byte array.
/// \param len - the length of the `data` array.
/// \param is_url whether to use the [URL safe alphabet](https://www.rfc-editor.org/rfc/rfc3548#section-4).
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn encode_base64(data: *const u8, len: usize, is_url: bool) -> *mut c_char {
    let data = std::slice::from_raw_parts(data, len);
    let config = base64::Config {
        url: is_url,
        pad: true,
    };
    let encoded = base64::encode(data, config);
    CString::new(encoded).unwrap().into_raw()
}

/// Decodes the base64 `data` string.
/// \param data *optional* C-compatible, nul-terminated string.
/// \param is_url whether to use the [URL safe alphabet](https://www.rfc-editor.org/rfc/rfc3548#section-4).
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn decode_base64(data: *const c_char, is_url: bool) -> CByteArrayResult {
    if data.is_null() {
        return CByteArrayResult::error(CEncodingCode::InvalidInput);
    }
    let str_slice = match CStr::from_ptr(data).to_str() {
        Ok(input) => input,
        Err(_) => return CByteArrayResult::error(CEncodingCode::InvalidInput),
    };
    let config = base64::Config {
        url: is_url,
        pad: true,
    };
    base64::decode(str_slice, config)
        .map(CByteArray::from)
        .map_err(CEncodingCode::from)
        .into()
}

/// Decodes the hex `data` string.
/// \param data *optional* C-compatible, nul-terminated string.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn decode_hex(data: *const c_char) -> CByteArrayResult {
    if data.is_null() {
        return CByteArrayResult::error(CEncodingCode::InvalidInput);
    }
    let hex_string = match CStr::from_ptr(data).to_str() {
        Ok(input) => input,
        Err(_) => return CByteArrayResult::error(CEncodingCode::InvalidInput),
    };

    hex::decode(hex_string)
        .map(CByteArray::from)
        .map_err(CEncodingCode::from)
        .into()
}

/// Encodes the octets `data` as a hex string using lowercase characters.
/// \param data *non-null* byte array.
/// \param len the length of the `data` array.
/// \param prefixed whether to add `0x` prefix.
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub unsafe extern "C" fn encode_hex(data: *const u8, len: usize, prefixed: bool) -> *mut c_char {
    let data = std::slice::from_raw_parts(data, len);
    let encoded = hex::encode(data, prefixed);
    CString::new(encoded).unwrap().into_raw()
}

fn get_alphabet(alphabet: *const c_char) -> Result<Option<String>, CEncodingCode> {
    if alphabet.is_null() {
        return Ok(None);
    }
    unsafe { CStr::from_ptr(alphabet).to_str() }
        .map(|alphabet| Some(alphabet.to_string()))
        .map_err(|_| CEncodingCode::InvalidAlphabet)
}
