// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#[derive(Debug, Clone, PartialEq, Eq, Copy)]
pub enum Error {
    /// Mnemonic has a word count that is not a multiple of 6.
    BadWordCount(usize),
    /// Mnemonic contains an unknown word.
    /// Error contains the index of the word.
    /// Use `mnemonic.split_whitespace().get(i)` to get the word.
    UnknownWord(usize),
    /// Entropy was not a multiple of 32 bits or between 128-256n bits in length.
    BadEntropyBitCount(usize),
    /// The mnemonic has an invalid checksum.
    InvalidChecksum,
}
