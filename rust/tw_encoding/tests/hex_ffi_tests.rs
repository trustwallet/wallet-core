// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::ffi::CString;
use tw_encoding::ffi::{decode_hex, encode_hex};

#[test]
fn test_encode_hex_without_prefix() {
    let data = b"hello world";
    let result_ptr = unsafe { encode_hex(data.as_ptr(), data.len(), false) };
    let result = unsafe { CString::from_raw(result_ptr) };
    let expected = "68656c6c6f20776f726c64";
    assert_eq!(result.to_str().unwrap(), expected);
}

#[test]
fn test_encode_hex_with_prefix() {
    let data = b"hello world";
    let result_ptr = unsafe { encode_hex(data.as_ptr(), data.len(), true) };
    let result = unsafe { CString::from_raw(result_ptr) };
    let expected = "0x68656c6c6f20776f726c64";
    assert_eq!(result.to_str().unwrap(), expected);
}

#[test]
fn test_decode_hex() {
    let encoded = "68656c6c6f20776f726c64";

    let encoded_c_str = CString::new(encoded).unwrap();
    let decoded: Vec<_> = unsafe { decode_hex(encoded_c_str.as_ptr()).unwrap().into_vec() };
    assert_eq!(decoded, b"hello world");
}

#[test]
fn test_decode_hex_with_prefix() {
    let encoded = "0x68656c6c6f20776f726c64";

    let encoded_c_str = CString::new(encoded).unwrap();
    let decoded: Vec<_> = unsafe { decode_hex(encoded_c_str.as_ptr()).unwrap().into_vec() };
    assert_eq!(decoded, b"hello world");
}
