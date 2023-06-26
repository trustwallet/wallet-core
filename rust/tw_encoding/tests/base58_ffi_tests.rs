// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::ffi::CString;
use tw_encoding::ffi::{decode_base58, encode_base58, Base58Alphabet};

#[test]
fn test_base58_encode() {
    let data = b"Hello, world!";
    let expected = "72k1xXWG59wUsYv7h2";

    let result_ptr = unsafe { encode_base58(data.as_ptr(), data.len(), Base58Alphabet::Bitcoin) };
    let result = unsafe { CString::from_raw(result_ptr) };
    assert_eq!(result.to_str().unwrap(), expected);
}

#[test]
fn test_base58_decode() {
    let data = "72k1xXWG59wUsYv7h2";
    let expected = b"Hello, world!";

    let input = CString::new(data).unwrap();
    let decoded = unsafe {
        decode_base58(input.as_ptr(), Base58Alphabet::Bitcoin)
            .unwrap()
            .into_vec()
    };
    assert_eq!(decoded, expected);
}
