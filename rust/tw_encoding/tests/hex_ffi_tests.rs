// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::{CStr, CString};
use tw_encoding::ffi::{decode_hex, encode_hex};

#[test]
fn test_encode_hex_without_prefix() {
    let data = b"hello world";
    let encoded = unsafe { CStr::from_ptr(encode_hex(data.as_ptr(), data.len(), false)) };
    let expected = "68656c6c6f20776f726c64";
    assert_eq!(encoded.to_str().unwrap(), expected);
}

#[test]
fn test_encode_hex_with_prefix() {
    let data = b"hello world";
    let encoded = unsafe { CStr::from_ptr(encode_hex(data.as_ptr(), data.len(), true)) };
    let expected = "0x68656c6c6f20776f726c64";
    assert_eq!(encoded.to_str().unwrap(), expected);
}

#[test]
fn test_decode_hex() {
    let encoded = "68656c6c6f20776f726c64";

    let encoded_c_str = CString::new(encoded).unwrap();
    let encoded_ptr = encoded_c_str.as_ptr();

    let decoded: Vec<_> = unsafe { decode_hex(encoded_ptr).unwrap().into_vec() };
    assert_eq!(decoded, b"hello world");
}

#[test]
fn test_decode_hex_with_prefix() {
    let encoded = "0x68656c6c6f20776f726c64";

    let encoded_c_str = CString::new(encoded).unwrap();
    let encoded_ptr = encoded_c_str.as_ptr();

    let decoded: Vec<_> = unsafe { decode_hex(encoded_ptr).unwrap().into_vec() };
    assert_eq!(decoded, b"hello world");
}
