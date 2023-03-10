// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use ripemd::{Ripemd160};
use crate::hash::hash_wrapper::hasher;
use crate::memory::CByteArray;

#[no_mangle]
pub extern "C" fn ripemd_160(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    hasher::<Ripemd160>(input).into()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_ripemd160() {
        let tests: Vec<(&[u8], String)> = vec![
            (b"hello world", String::from("98c615784ccb5fe5936fbc0cbe9dfdb408d92f0f")),
            (b"The quick brown fox jumps over the lazy dog", String::from("37f332f68db77bd9d7edd4969571ad671cf9dd3b")),
        ];
        for test in tests {
            let result = ripemd_160(test.0.as_ptr(), test.0.len());
            let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
            assert_eq!(hex::encode(decoded_slice), test.1);
        }
    }
}
