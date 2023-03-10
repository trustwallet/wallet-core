// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use blake_hash::{Blake256, Digest};
use crate::memory::CByteArray;

#[no_mangle]
pub extern "C" fn blake_256(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    let mut hasher = Blake256::new();
    hasher.update(input);
    let result = &hasher.finalize()[..];
    result.to_vec().into()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_blake_256() {
        let tests: Vec<(&[u8], String)> = vec![
            (b"hello world", String::from("4a6f552c9a404582b4eeff4364df2d5a0da9e3f7635c7b638bacab9f5d09bdb1")),
            (b"The quick brown fox jumps over the lazy dog", String::from("7576698ee9cad30173080678e5965916adbb11cb5245d386bf1ffda1cb26c9d7")),
        ];
        for test in tests {
            let result = blake_256(test.0.as_ptr(), test.0.len());
            let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
            assert_eq!(hex::encode(decoded_slice), test.1);
        }
    }
}
