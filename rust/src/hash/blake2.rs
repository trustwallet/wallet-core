// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use blake2::{
    Blake2bVar,
    digest::{Update, VariableOutput}
};
use blake2b_ref::{Blake2bBuilder};
use crate::memory::CByteArray;

#[no_mangle]
pub extern "C" fn blake2_b(input: *const u8, input_len: usize, hash_size: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    let mut hasher = Blake2bVar::new(hash_size).unwrap();
    hasher.update(input);
    let mut buf = Vec::with_capacity(hash_size);
    buf.resize(hash_size, 0);
    hasher.finalize_variable(&mut buf).unwrap();
    buf.into()
}

#[no_mangle]
pub extern "C" fn blake2_b_personal(input: *const u8, input_len: usize, hash_size: usize, personal_input: *const u8, personal_len: usize) -> CByteArray {
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    let personal = unsafe { std::slice::from_raw_parts(personal_input, personal_len) };

    let mut output: Vec<u8> = vec![];
    output.resize(hash_size, 0);
    let mut blake2b = Blake2bBuilder::new(hash_size).personal(personal).build();
    blake2b.update(input);
    blake2b.finalize(&mut output);
    output.into()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_blake2b() {
        let tests: Vec<(&[u8], String)> = vec![
            (b"Hello world", String::from("6ff843ba685842aa82031d3f53c48b66326df7639a63d128974c5c14f31a0f33343a8c65551134ed1ae0f2b0dd2bb495dc81039e3eeb0aa1bb0388bbeac29183")),
            (b"The quick brown fox jumps over the lazy dog", String::from("a8add4bdddfd93e4877d2746e62817b116364a1fa7bc148d95090bc7333b3673f82401cf7aa2e4cb1ecd90296e3f14cb5413f8ed77be73045b13914cdcd6a918")),
        ];
        for test in tests {
            let result = blake2_b(test.0.as_ptr(), test.0.len(), 64);
            let decoded_slice = unsafe { std::slice::from_raw_parts(result.data, result.size) };
            assert_eq!(hex::encode(decoded_slice), test.1);
        }
    }

    #[test]
    fn test_blake2b_personal() {
        let personal_data = b"MyApp Files Hash";
        let content = b"the same content";
        let result = blake2_b_personal(content.as_ptr(), content.len(), 32, personal_data.as_ptr(), personal_data.len());
        let result = unsafe { std::slice::from_raw_parts(result.data, result.size) };
        let expected = "20d9cd024d4fb086aae819a1432dd2466de12947831b75c5a30cf2676095d3b4".to_string();
        assert_eq!(hex::encode(result), expected);
    }
}
