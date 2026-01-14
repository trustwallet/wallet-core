// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::crypto_mnemonic::mnemonic::Mnemonic;
use tw_macros::tw_ffi;
use tw_memory::ffi::{
    tw_data::TWData, tw_string::TWString, Nonnull, NonnullMut, NullableMut, RawPtrTrait,
};
use tw_misc::{try_or_else, try_or_false};

#[tw_ffi(ty = static_function, class = TWMnemonic, name = Generate)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_generate(strength: i32) -> NullableMut<TWString> {
    let strength: u32 = try_or_else!(strength.try_into(), std::ptr::null_mut);
    let mnemonic = try_or_else!(Mnemonic::generate(strength), std::ptr::null_mut);
    TWString::from(mnemonic.to_string_zeroizing()).into_ptr()
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = GenerateFromData)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_generate_from_data(
    data: Nonnull<TWData>,
) -> NullableMut<TWString> {
    let data = TWData::from_ptr_as_ref(data)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    let mnemonic = try_or_else!(Mnemonic::generate_from_data(data), std::ptr::null_mut);
    TWString::from(mnemonic.to_string_zeroizing()).into_ptr()
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = IsValid)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_is_valid(mnemonic: Nonnull<TWString>) -> bool {
    let mnemonic_string = try_or_false!(TWString::from_ptr_as_ref(mnemonic));
    let mnemonic_string = try_or_false!(mnemonic_string.as_str());
    Mnemonic::is_valid(mnemonic_string)
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = IsValidWord)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_is_valid_word(word: Nonnull<TWString>) -> bool {
    let word_string = try_or_false!(TWString::from_ptr_as_ref(word));
    let word_string = try_or_false!(word_string.as_str());
    Mnemonic::is_valid_word(word_string)
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = GetWord)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_get_word(index: i32) -> NullableMut<TWString> {
    let index: u32 = try_or_else!(index.try_into(), std::ptr::null_mut);
    let word = try_or_else!(Mnemonic::get_word(index), std::ptr::null_mut);
    TWString::from(word.to_string()).into_ptr()
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = FindWord)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_find_word(word: Nonnull<TWString>) -> i32 {
    let word_string = try_or_else!(TWString::from_ptr_as_ref(word), || -1);
    let word_string = try_or_else!(word_string.as_str(), || -1);
    let index = Mnemonic::find_word(word_string);
    index.map(|index| index as i32).unwrap_or(-1)
}

#[tw_ffi(ty = static_function, class = TWMnemonic, name = Suggest)]
#[no_mangle]
pub unsafe extern "C" fn tw_mnemonic_suggest(prefix: Nonnull<TWString>) -> NonnullMut<TWString> {
    let prefix_string = try_or_else!(TWString::from_ptr_as_ref(prefix), std::ptr::null_mut);
    let prefix_string = try_or_else!(prefix_string.as_str(), std::ptr::null_mut);
    let words_string = Mnemonic::suggest(prefix_string);
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
    let seed = Mnemonic::to_seed(mnemonic_string, passphrase_string);
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
