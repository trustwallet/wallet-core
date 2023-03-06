// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use sha2::{Sha256};
use crate::hashes::hash_wrapper::hash_wrapper;
use crate::memory::CByteArray;

#[no_mangle]
pub extern "C" fn sha256(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    hash_wrapper::<Sha256>(input).into()
}

#[test]
fn test_sha256() {
    let tests: Vec<(&[u8], String)> = vec![
        (b"hello world", String::from("b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9")),
        (b"The quick brown fox jumps over the lazy dog", String::from("d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592")),
    ];
    for test in tests {
        let result = sha256(test.0.as_ptr(), test.0.len());
        let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
        assert_eq!(hex::encode(decoded_slice), test.1);
    }
}
