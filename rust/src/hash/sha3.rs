// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use sha3::{Keccak256, Keccak512, Sha3_256, Sha3_512};
use crate::hash::hash_wrapper::hasher;
use crate::memory::CByteArray;

#[no_mangle]
pub extern "C" fn keccak256(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    hasher::<Keccak256>(input).into()
}

#[no_mangle]
pub extern "C" fn keccak512(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    hasher::<Keccak512>(input).into()
}

#[no_mangle]
pub extern "C" fn sha3__256(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    hasher::<Sha3_256>(input).into()
}

#[no_mangle]
pub extern "C" fn sha3__512(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    hasher::<Sha3_512>(input).into()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_keccak256() {
        let tests: Vec<(&[u8], String)> = vec![
            (b"hello world", String::from("47173285a8d7341e5e972fc677286384f802f8ef42a5ec5f03bbfa254cb01fad")),
            (b"The quick brown fox jumps over the lazy dog", String::from("4d741b6f1eb29cb2a9b9911c82f56fa8d73b04959d3d9d222895df6c0b28aa15")),
        ];
        for test in tests {
            let result = keccak256(test.0.as_ptr(), test.0.len());
            let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
            assert_eq!(hex::encode(decoded_slice), test.1);
        }
    }

    #[test]
    fn test_keccak512() {
        let tests: Vec<(&[u8], String)> = vec![
            (b"hello world", String::from("3ee2b40047b8060f68c67242175660f4174d0af5c01d47168ec20ed619b0b7c42181f40aa1046f39e2ef9efc6910782a998e0013d172458957957fac9405b67d")),
            (b"The quick brown fox jumps over the lazy dog", String::from("d135bb84d0439dbac432247ee573a23ea7d3c9deb2a968eb31d47c4fb45f1ef4422d6c531b5b9bd6f449ebcc449ea94d0a8f05f62130fda612da53c79659f609")),
        ];
        for test in tests {
            let result = keccak512(test.0.as_ptr(), test.0.len());
            let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
            assert_eq!(hex::encode(decoded_slice), test.1);
        }
    }

    #[test]
    fn test_sha3_256() {
        let tests: Vec<(&[u8], String)> = vec![
            (b"hello world", String::from("644bcc7e564373040999aac89e7622f3ca71fba1d972fd94a31c3bfbf24e3938")),
            (b"The quick brown fox jumps over the lazy dog", String::from("69070dda01975c8c120c3aada1b282394e7f032fa9cf32f4cb2259a0897dfc04")),
        ];
        for test in tests {
            let result = sha3__256(test.0.as_ptr(), test.0.len());
            let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
            assert_eq!(hex::encode(decoded_slice), test.1);
        }
    }

    #[test]
    fn test_sha3_512() {
        let tests: Vec<(&[u8], String)> = vec![
            (b"hello world", String::from("840006653e9ac9e95117a15c915caab81662918e925de9e004f774ff82d7079a40d4d27b1b372657c61d46d470304c88c788b3a4527ad074d1dccbee5dbaa99a")),
            (b"The quick brown fox jumps over the lazy dog", String::from("01dedd5de4ef14642445ba5f5b97c15e47b9ad931326e4b0727cd94cefc44fff23f07bf543139939b49128caf436dc1bdee54fcb24023a08d9403f9b4bf0d450")),
        ];
        for test in tests {
            let result = sha3__512(test.0.as_ptr(), test.0.len());
            let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
            assert_eq!(hex::encode(decoded_slice), test.1);
        }
    }
}
