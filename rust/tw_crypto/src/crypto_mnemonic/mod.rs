// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

mod mnemonic_pbkdf2;

pub fn to_seed<I>(words: I, passphrase: &str) -> [u8; 64]
where
    I: Iterator<Item = &'static str> + Clone,
{
    const PBKDF2_ROUNDS: usize = 2048;
    const PBKDF2_BYTES: usize = 64;

    let mut seed = [0u8; PBKDF2_BYTES];
    mnemonic_pbkdf2::pbkdf2(words, passphrase.as_bytes(), PBKDF2_ROUNDS, &mut seed);
    seed
}
