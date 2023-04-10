// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::CString;
use tw_encoding::ffi::{decode_base32, encode_base32};

/// Checks if the encoded `input` with the given `alphabet` and `padding` parameters
/// equals to `expected`.
#[track_caller]
fn test_base32_encode_helper(input: &[u8], expected: &str, alphabet: Option<&str>, padding: bool) {
    let alphabet = alphabet
        .map(|alphabet| CString::new(alphabet).unwrap().into_raw())
        .unwrap_or_else(std::ptr::null_mut);

    let result_ptr =
        unsafe { encode_base32(input.as_ptr(), input.len(), alphabet, padding) }.unwrap();
    let result = unsafe { CString::from_raw(result_ptr) };
    assert_eq!(result.to_str().unwrap(), expected);
}

/// Checks if the decoded `input` with the given `alphabet` and `padding` parameters
/// equals to `expected`.
#[track_caller]
fn test_base32_decode_helper(input: &str, expected: &[u8], alphabet: Option<&str>, padding: bool) {
    let input = CString::new(input).unwrap();
    let alphabet = alphabet
        .map(|alphabet| CString::new(alphabet).unwrap().into_raw())
        .unwrap_or_else(std::ptr::null_mut);

    let decoded = unsafe {
        decode_base32(input.as_ptr(), alphabet, padding)
            .unwrap()
            .into_vec()
    };
    assert_eq!(decoded, expected);
}

#[test]
fn test_base32_encode() {
    let input = b"Hello, world!";
    let alphabet = "abcdefghijklmnopqrstuvwxyz234567";

    test_base32_encode_helper(input, "JBSWY3DPFQQHO33SNRSCC", None, false);
    test_base32_encode_helper(input, "jbswy3dpfqqho33snrscc===", Some(alphabet), true);
    test_base32_encode_helper(input, "JBSWY3DPFQQHO33SNRSCC===", None, true);
    test_base32_encode_helper(input, "jbswy3dpfqqho33snrscc", Some(alphabet), false);
}

#[test]
fn test_base32_decode() {
    let expected = b"Hello, world!";
    let alphabet = "abcdefghijklmnopqrstuvwxyz234567";

    test_base32_decode_helper("JBSWY3DPFQQHO33SNRSCC===", expected, None, true);
    test_base32_decode_helper("JBSWY3DPFQQHO33SNRSCC", expected, None, false);
    test_base32_decode_helper("jbswy3dpfqqho33snrscc===", expected, Some(alphabet), true);
    test_base32_decode_helper("jbswy3dpfqqho33snrscc", expected, Some(alphabet), false);
}
