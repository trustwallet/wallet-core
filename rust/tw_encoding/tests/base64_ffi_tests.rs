// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::ffi::CString;
use tw_encoding::ffi::{decode_base64, encode_base64};

#[test]
fn test_encode_base64() {
    let data = b"hello world";
    let result_ptr = unsafe { encode_base64(data.as_ptr(), data.len(), false) };
    let result = unsafe { CString::from_raw(result_ptr) };
    let expected = "aGVsbG8gd29ybGQ=";
    assert_eq!(result.to_str().unwrap(), expected);
}

#[test]
fn test_encode_base64_url() {
    let data = b"+'?ab";
    let result_ptr = unsafe { encode_base64(data.as_ptr(), data.len(), true) };
    let result = unsafe { CString::from_raw(result_ptr) };
    let expected = "Kyc_YWI=";
    assert_eq!(result.to_str().unwrap(), expected);
}

#[test]
fn test_decode_base64_url() {
    let encoded = "Kyc_YWI=";
    let expected = b"+'?ab";

    let encoded_c_str = CString::new(encoded).unwrap();
    let decoded = unsafe {
        decode_base64(encoded_c_str.as_ptr(), true)
            .unwrap()
            .into_vec()
    };
    assert_eq!(decoded, expected);
}

#[test]
fn test_decode_base64() {
    let encoded = "aGVsbG8gd29ybGQh";
    let expected = b"hello world!";

    let encoded_c_str = CString::new(encoded).unwrap();
    let decoded = unsafe {
        decode_base64(encoded_c_str.as_ptr(), false)
            .unwrap()
            .into_vec()
    };
    assert_eq!(decoded, expected);
}

#[test]
fn test_decode_base64_invalid() {
    let invalid_encoded = "_This_is_an_invalid_base64_";
    let encoded_c_str = CString::new(invalid_encoded).unwrap();
    let res = unsafe { decode_base64(encoded_c_str.as_ptr(), false) };
    assert!(res.is_err());
}
