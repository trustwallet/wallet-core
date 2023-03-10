// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use groestl::{Groestl512};
use crate::hash::hash_wrapper::hasher;
use crate::memory::CByteArray;

#[no_mangle]
pub extern "C" fn groestl_512(input: *const u8, input_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    hasher::<Groestl512>(input).into()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_groestl_512() {
        let tests: Vec<(&[u8], String)> = vec![
            (b"hello world", String::from("0059e9c916018f0e5efbe6e9376f7562203eb4b2c9267ba30353f1f0cfd4a950e7ae68d9e7cda002432c7f3efdf2a4c0cd673ddb9109e7d846538414d4a72c36")),
            (b"The quick brown fox jumps over the lazy dog", String::from("badc1f70ccd69e0cf3760c3f93884289da84ec13c70b3d12a53a7a8a4a513f99715d46288f55e1dbf926e6d084a0538e4eebfc91cf2b21452921ccde9131718d")),
        ];
        for test in tests {
            let result = groestl_512(test.0.as_ptr(), test.0.len());
            let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
            assert_eq!(hex::encode(decoded_slice), test.1);
        }
    }
}
