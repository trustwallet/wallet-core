// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::crypto_mnemonic::to_seed;
use bip39::{Language, Mnemonic};
use tw_macros::tw_ffi;
use tw_memory::ffi::{
    tw_data::TWData, tw_string::TWString, Nonnull, NonnullMut, NullableMut, RawPtrTrait,
};
use tw_misc::{try_or_else, try_or_false};

const SUGGEST_MAX_COUNT: usize = 10;

#[tw_ffi(ty = static_function, class = TWMnemonic, name = Generate)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_generate(strength: u32) -> NullableMut<TWString> {
    if strength % 32 != 0 || !(128..=256).contains(&strength) {
        return std::ptr::null_mut();
    }

    let length = strength / 8;
    let mnemonic_length = length * 3 / 4;

    let mut rng = bip39::rand::thread_rng();
    let mnemonic = try_or_else!(
        Mnemonic::generate_in_with(&mut rng, Language::English, mnemonic_length as usize),
        std::ptr::null_mut
    );

    let mnemonic_string = mnemonic.to_string();
    TWString::from(mnemonic_string).into_ptr()
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = GenerateFromData)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_generate_from_data(
    data: Nonnull<TWData>,
) -> NullableMut<TWString> {
    let data = TWData::from_ptr_as_ref(data)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    let mnemonic = try_or_else!(Mnemonic::from_entropy(data), std::ptr::null_mut);

    let mnemonic_string = mnemonic.to_string();
    TWString::from(mnemonic_string).into_ptr()
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = IsValid)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_is_valid(mnemonic: Nonnull<TWString>) -> bool {
    let mnemonic_string = try_or_false!(TWString::from_ptr_as_ref(mnemonic));
    let mnemonic_string = try_or_false!(mnemonic_string.as_str());
    if mnemonic_string.trim() != mnemonic_string || mnemonic_string.contains("  ") {
        return false;
    }
    Mnemonic::parse_in(Language::English, mnemonic_string).is_ok()
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = IsValidWord)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_is_valid_word(word: Nonnull<TWString>) -> bool {
    let word_string = try_or_false!(TWString::from_ptr_as_ref(word));
    let word_string = try_or_false!(word_string.as_str());
    let language = Language::English;
    language.find_word(word_string).is_some()
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = GetWord)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_get_word(index: u32) -> NullableMut<TWString> {
    let language = Language::English;
    let word = try_or_else!(language.word_list().get(index as usize), std::ptr::null_mut);
    TWString::from(word.to_string()).into_ptr()
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = FindWord)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_find_word(word: Nonnull<TWString>) -> i32 {
    let word_string = try_or_else!(TWString::from_ptr_as_ref(word), || -1);
    let word_string = try_or_else!(word_string.as_str(), || -1);
    let language = Language::English;
    language
        .find_word(word_string)
        .map(|index| index as i32)
        .unwrap_or(-1)
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = Suggest)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_suggest(prefix: Nonnull<TWString>) -> NonnullMut<TWString> {
    let empty_string = TWString::from("".to_string()).into_ptr();
    let prefix_string = try_or_else!(TWString::from_ptr_as_ref(prefix), || empty_string);
    let prefix_string = try_or_else!(prefix_string.as_str(), || empty_string);
    if prefix_string.is_empty() {
        return empty_string;
    }
    let language = Language::English;
    let prefix_string = prefix_string.to_lowercase();
    let words = language.words_by_prefix(&prefix_string);
    let words_string = words
        .iter()
        .take(SUGGEST_MAX_COUNT)
        .map(|w| w.to_string())
        .collect::<Vec<_>>()
        .join(" ");
    TWString::from(words_string).into_ptr()
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = ToSeed)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_to_seed(
    mnemonic: Nonnull<TWString>,
    passphrase: Nonnull<TWString>,
) -> NullableMut<TWData> {
    let mnemonic_string = try_or_else!(TWString::from_ptr_as_ref(mnemonic), std::ptr::null_mut);
    let mnemonic_string = try_or_else!(mnemonic_string.as_str(), std::ptr::null_mut);
    let passphrase_string = try_or_else!(TWString::from_ptr_as_ref(passphrase), std::ptr::null_mut);
    let passphrase_string = try_or_else!(passphrase_string.as_str(), std::ptr::null_mut);
    let words = mnemonic_string.split_whitespace();
    let seed = to_seed(words, passphrase_string);
    TWData::from(seed.to_vec()).into_ptr()
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = ToEntropy)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_to_entropy(
    mnemonic: Nonnull<TWString>,
) -> NullableMut<TWData> {
    let mnemonic_string = try_or_else!(TWString::from_ptr_as_ref(mnemonic), std::ptr::null_mut);
    let mnemonic_string = try_or_else!(mnemonic_string.as_str(), std::ptr::null_mut);
    let mnemonic = try_or_else!(Mnemonic::parse(mnemonic_string), || TWData::from(vec![])
        .into_ptr());
    let entropy = mnemonic.to_entropy();
    TWData::from(entropy).into_ptr()
}
