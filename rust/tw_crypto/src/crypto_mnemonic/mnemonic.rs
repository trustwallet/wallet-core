// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::crypto_mnemonic::to_seed;
use bip39::{Error, Language, Mnemonic as Bip39Mnemonic};
use std::fmt;

const SUGGEST_MAX_COUNT: usize = 10;

pub struct Mnemonic {
    mnemonic: Bip39Mnemonic,
}

impl Mnemonic {
    pub fn generate(strength: u32) -> Result<Self, Error> {
        if strength % 32 != 0 || !(128..=256).contains(&strength) {
            return Err(Error::BadEntropyBitCount(strength as usize));
        }

        let length = strength / 8;
        let mnemonic_length = length * 3 / 4;

        let mut rng = bip39::rand::thread_rng();
        let mnemonic =
            Bip39Mnemonic::generate_in_with(&mut rng, Language::English, mnemonic_length as usize)?;

        Ok(Self { mnemonic })
    }

    pub fn generate_from_data(data: &[u8]) -> Result<Self, Error> {
        let mnemonic = Bip39Mnemonic::from_entropy(data)?;
        Ok(Self { mnemonic })
    }

    pub fn parse(mnemonic: &str) -> Result<Self, Error> {
        if mnemonic.trim() != mnemonic || mnemonic.contains("  ") {
            return Err(Error::InvalidChecksum);
        }
        let mnemonic = Bip39Mnemonic::parse_in(Language::English, mnemonic)?;
        Ok(Self { mnemonic })
    }

    pub fn is_valid(mnemonic: &str) -> bool {
        Self::parse(mnemonic).is_ok()
    }

    pub fn is_valid_word(word: &str) -> bool {
        let language = Language::English;
        language.find_word(word).is_some()
    }

    pub fn get_word(index: u32) -> Option<String> {
        let language = Language::English;
        language
            .word_list()
            .get(index as usize)
            .map(|w| w.to_string())
    }

    pub fn find_word(word: &str) -> Option<u32> {
        let language = Language::English;
        language.find_word(word).map(|index| index as u32)
    }

    pub fn suggest(prefix: &str) -> String {
        if prefix.is_empty() {
            return "".to_string();
        }
        let language = Language::English;
        let prefix_string = prefix.to_lowercase();
        let words = language.words_by_prefix(&prefix_string);
        let words_string = words
            .iter()
            .take(SUGGEST_MAX_COUNT)
            .map(|w| w.to_string())
            .collect::<Vec<_>>()
            .join(" ");
        words_string
    }

    pub fn to_seed(mnemonic: &'static str, passphrase: &str) -> [u8; 64] {
        to_seed(mnemonic.split_whitespace(), passphrase)
    }

    pub fn to_entropy(&self) -> Vec<u8> {
        self.mnemonic.to_entropy()
    }
}

impl fmt::Display for Mnemonic {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.mnemonic)
    }
}
