// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

// Taken from https://github.com/rust-bitcoin/rust-bip39/blob/master/src/language/mod.rs

use super::english;

#[derive(Clone, Copy, Debug, Hash, PartialEq, Eq, PartialOrd, Ord)]
pub enum Language {
    /// The English language.
    English,
}

impl Language {
    pub fn word_list(self) -> &'static [&'static str; 2048] {
        match self {
            Language::English => &english::WORDS,
        }
    }

    /// Get the index of the word in the word list.
    pub fn find_word(&self, word: &str) -> Option<u16> {
        self.word_list().binary_search(&word).map(|x| x as _).ok()
    }

    /// Get words from the word list that start with the given prefix.
    pub fn words_by_prefix(self, prefix: &str) -> &[&'static str] {
        // The words in the word list are ordered lexicographically. This means
        // that we cannot use `binary_search` to find words more efficiently,
        // because the Rust ordering is based on the byte values. However, it
        // does mean that words that share a prefix will follow each other.

        let first = match self.word_list().iter().position(|w| w.starts_with(prefix)) {
            Some(i) => i,
            None => return &[],
        };
        let count = self.word_list()[first..]
            .iter()
            .take_while(|w| w.starts_with(prefix))
            .count();
        &self.word_list()[first..first + count]
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn words_by_prefix() {
        let lang = Language::English;

        let res = lang.words_by_prefix("woo");
        assert_eq!(res, ["wood", "wool"]);

        let res = lang.words_by_prefix("");
        assert_eq!(res.len(), 2048);

        let res = lang.words_by_prefix("woof");
        assert!(res.is_empty());
    }

    #[test]
    fn words_overlaps() {
        use std::collections::HashMap;

        // We keep a map of all words and the languages they occur in.
        // Afterwards, we make sure that no word maps to multiple languages
        // if either of those is guaranteed to have unique words.
        let mut words: HashMap<&str, Vec<Language>> = HashMap::new();
        for lang in [Language::English].iter() {
            for word in lang.word_list().iter() {
                words.entry(word).or_insert(Vec::new()).push(*lang);
            }
        }

        let mut ok = true;
        for (word, langs) in words.into_iter() {
            if langs.len() == 1 {
                continue;
            }
            if langs.iter().any(|_| false) {
                println!("Word {} is not unique: {:?}", word, langs);
                ok = false;
            }
        }
        assert!(ok);
    }

    #[test]
    fn test_ordered_lists() {
        let languages = [Language::English];

        for lang in languages.iter() {
            let mut list = lang.word_list().to_vec();
            list.sort();
            assert_eq!(&list[..], &lang.word_list()[..]);
        }
    }

    /// Test the full round trip from index -> word-string -> index for all langauges
    mod round_trip {
        use super::*;

        #[test]
        fn english() {
            for i in 0..0x800 {
                let word_str = Language::English.word_list()[i];
                assert_eq!(Language::English.find_word(word_str), Some(i as _));
            }
        }
    }
}
