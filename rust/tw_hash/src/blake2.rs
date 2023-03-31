// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use blake2::{
    digest::{Update, VariableOutput},
    Blake2bVar,
};
use blake2b_ref::Blake2bBuilder;

pub fn blake2_b(input: &[u8], hash_size: usize) -> Vec<u8> {
    let mut hasher = Blake2bVar::new(hash_size).unwrap();
    hasher.update(input);
    let mut buf = vec![0; hash_size];
    hasher.finalize_variable(&mut buf).unwrap();
    buf
}

pub fn blake2_b_personal(input: &[u8], hash_size: usize, personal_input: &[u8]) -> Vec<u8> {
    let mut output: Vec<u8> = vec![0; hash_size];
    let mut blake2b = Blake2bBuilder::new(hash_size)
        .personal(personal_input)
        .build();
    blake2b.update(input);
    blake2b.finalize(&mut output);
    output
}
