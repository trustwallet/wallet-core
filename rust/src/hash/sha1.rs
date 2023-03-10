// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use sha1::{Sha1};
use crate::hash::hash_wrapper::hasher;
use crate::memory::CByteArray;

#[no_mangle]
pub extern "C" fn sha1(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    hasher::<Sha1>(input).into()
}

#[test]
fn test_sha1() {
    let tests: Vec<(&[u8], String)> = vec![
        (b"", String::from("da39a3ee5e6b4b0d3255bfef95601890afd80709")),
        (b"The quick brown fox jumps over the lazy dog", String::from("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12")),
        (b"The quick brown fox jumps over the lazy dog.", String::from("408d94384216f890ff7a0c3528e8bed1e0b01621")),
    ];
    for test in tests {
        let result = sha1(test.0.as_ptr(), test.0.len());
        let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
        assert_eq!(hex::encode(decoded_slice), test.1);
    }
}
