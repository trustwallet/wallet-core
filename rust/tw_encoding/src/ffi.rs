// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{base32, base58, base64, hex};
use bs58::Alphabet;
use std::ffi::{c_char, CStr, CString};
use tw_memory::ffi::CByteArray;

#[repr(C)]
#[derive(PartialEq, Debug)]
pub enum Base58Alphabet {
    Bitcoin = 1,
    Ripple = 2,
}

impl From<Base58Alphabet> for &Alphabet {
    fn from(value: Base58Alphabet) -> Self {
        match value {
            Base58Alphabet::Bitcoin => Alphabet::BITCOIN,
            Base58Alphabet::Ripple => Alphabet::RIPPLE,
        }
    }
}

/// Encodes the `input` data as base32.
/// \param input *non-null* byte array.
/// \param alphabet *optional* C-compatible, nul-terminated string.
///                `ALPHABET_RFC4648` is used by default if `alphabet` is null.
/// \param padding whether the padding bytes should be included.
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub extern "C" fn encode_base32(
    input: *const u8,
    input_len: usize,
    alphabet: *const c_char,
    padding: bool,
) -> *mut c_char {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };

    let alphabet = get_alphabet(alphabet);

    base32::encode(input, alphabet, padding)
        .map(|result| CString::new(result).unwrap().into_raw())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Decodes the base32 `input` string.
/// \param input *non-null* C-compatible, nul-terminated string.
/// \param alphabet *optional* C-compatible, nul-terminated string.
///                `ALPHABET_RFC4648` is used by default if `alphabet` is null.
/// \param padding whether the padding bytes should be trimmed when decoding.
/// \return C-compatible byte array.
#[no_mangle]
pub extern "C" fn decode_base32(
    input: *const c_char,
    alphabet: *const c_char,
    padding: bool,
) -> CByteArray {
    let input = unsafe { CStr::from_ptr(input).to_str().unwrap() };
    let alphabet = get_alphabet(alphabet);

    base32::decode(input, alphabet, padding)
        .map(CByteArray::from)
        .unwrap_or_else(|_| CByteArray::null())
}

/// Encodes the `input` data as base58.
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \param alphabet alphabet type.
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub extern "C" fn encode_base58(
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
/// \return C-compatible byte array.
#[no_mangle]
pub extern "C" fn decode_base58(input: *const c_char, alphabet: Base58Alphabet) -> CByteArray {
    let input = unsafe { CStr::from_ptr(input).to_str().unwrap() };

    base58::decode(input, alphabet.into())
        .map(CByteArray::from)
        .unwrap_or_else(|_| CByteArray::null())
}

/// Encodes the `data` data as a padded, base64 string.
/// \param data *non-null* byte array.
/// \param len - the length of the `data` array.
/// \param is_url whether to use the [URL safe alphabet](https://www.rfc-editor.org/rfc/rfc3548#section-4).
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub extern "C" fn encode_base64(data: *const u8, len: usize, is_url: bool) -> *mut c_char {
    let data = unsafe { std::slice::from_raw_parts(data, len) };
    let encoded = base64::encode(data, is_url);
    CString::new(encoded).unwrap().into_raw()
}

/// Decodes the base64 `data` string.
/// \param data *optional* C-compatible, nul-terminated string.
/// \param is_url whether to use the [URL safe alphabet](https://www.rfc-editor.org/rfc/rfc3548#section-4).
/// \return C-compatible byte array.
#[no_mangle]
pub extern "C" fn decode_base64(data: *const c_char, is_url: bool) -> CByteArray {
    if data.is_null() {
        return CByteArray::null();
    }
    let c_str = unsafe { CStr::from_ptr(data) };
    let str_slice = c_str.to_str().unwrap();
    base64::decode(str_slice, is_url)
        .map(CByteArray::from)
        .unwrap_or_else(|_| CByteArray::null())
}

/// Decodes the hex `data` string.
/// \param data *optional* C-compatible, nul-terminated string.
/// \return C-compatible byte array.
#[no_mangle]
pub extern "C" fn decode_hex(data: *const c_char) -> CByteArray {
    if data.is_null() {
        return CByteArray {
            data: std::ptr::null_mut(),
            size: 0,
        };
    }
    let c_str = unsafe { CStr::from_ptr(data) };
    let hex_string = c_str.to_str().unwrap();

    return hex::decode(hex_string)
        .map(CByteArray::from)
        .unwrap_or_else(|_| CByteArray::null());
}

/// Encodes the octets `data` as a hex string using lowercase characters.
/// \param data *non-null* byte array.
/// \param len the length of the `data` array.
/// \param prefixed whether to add `0x` prefix.
/// \return *non-null* C-compatible, nul-terminated string.
#[no_mangle]
pub extern "C" fn encode_hex(data: *const u8, len: usize, prefixed: bool) -> *mut c_char {
    let data = unsafe { std::slice::from_raw_parts(data, len) };
    let encoded = hex::encode(data, prefixed);
    CString::new(encoded).unwrap().into_raw()
}

fn get_alphabet(alphabet: *const c_char) -> Option<&'static [u8]> {
    if alphabet.is_null() {
        return None;
    }
    let alphabet = unsafe { CStr::from_ptr(alphabet).to_str().unwrap().as_bytes() };
    Some(alphabet)
}
