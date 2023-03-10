// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use sha2::{Sha256, Sha512, Sha512_256};
use crate::hash::hash_wrapper::hasher;
use crate::memory::CByteArray;

#[no_mangle]
pub extern "C" fn sha256(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    hasher::<Sha256>(input).into()
}

#[no_mangle]
pub extern "C" fn sha512(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    hasher::<Sha512>(input).into()
}

#[no_mangle]
pub extern "C" fn sha512_256(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    hasher::<Sha512_256>(input).into()
}

#[cfg(test)]
mod tests {
    use super::*;

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

    #[test]
    fn test_sha512() {
        let tests: Vec<(&[u8], String)> = vec![
            (b"hello world", String::from("309ecc489c12d6eb4cc40f50c902f2b4d0ed77ee511a7c7a9bcd3ca86d4cd86f989dd35bc5ff499670da34255b45b0cfd830e81f605dcf7dc5542e93ae9cd76f")),
            (b"The quick brown fox jumps over the lazy dog", String::from("07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb642e93a252a954f23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6")),
        ];
        for test in tests {
            let result = sha512(test.0.as_ptr(), test.0.len());
            let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
            assert_eq!(hex::encode(decoded_slice), test.1);
        }
    }

    #[test]
    fn test_sha512_256() {
        let tests: Vec<(&[u8], String)> = vec![
            (b"hello world", String::from("0ac561fac838104e3f2e4ad107b4bee3e938bf15f2b15f009ccccd61a913f017")),
            (b"The quick brown fox jumps over the lazy dog", String::from("dd9d67b371519c339ed8dbd25af90e976a1eeefd4ad3d889005e532fc5bef04d")),
        ];
        for test in tests {
            let result = sha512_256(test.0.as_ptr(), test.0.len());
            let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
            assert_eq!(hex::encode(decoded_slice), test.1);
        }
    }
}
