// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::crypto_aes_cbc::{
    aes_cbc_decrypt, aes_cbc_decrypt_128, aes_cbc_decrypt_192, aes_cbc_decrypt_256,
    aes_cbc_encrypt, aes_cbc_encrypt_128, aes_cbc_encrypt_192, aes_cbc_encrypt_256,
    padding::PaddingMode,
};
use tw_macros::tw_ffi;
use tw_memory::ffi::{tw_data::TWData, Nonnull, NullableMut, RawPtrTrait};
use tw_misc::try_or_else;

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCbcEncrypt128)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_cbc_encrypt_128(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
    padding_mode: u32,
) -> NullableMut<TWData> {
    let padding_mode = try_or_else!(PaddingMode::try_from(padding_mode), std::ptr::null_mut);
    let data = TWData::from_ptr_as_ref(data)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let iv = TWData::from_ptr_as_ref(iv)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let key = TWData::from_ptr_as_ref(key)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    aes_cbc_encrypt_128(data, iv, key, padding_mode)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCbcDecrypt128)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_cbc_decrypt_128(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
    padding_mode: u32,
) -> NullableMut<TWData> {
    let padding_mode = try_or_else!(PaddingMode::try_from(padding_mode), std::ptr::null_mut);
    let data = TWData::from_ptr_as_ref(data)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let iv = TWData::from_ptr_as_ref(iv)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let key = TWData::from_ptr_as_ref(key)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    aes_cbc_decrypt_128(data, iv, key, padding_mode)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCbcEncrypt192)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_cbc_encrypt_192(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
    padding_mode: u32,
) -> NullableMut<TWData> {
    let padding_mode = try_or_else!(PaddingMode::try_from(padding_mode), std::ptr::null_mut);
    let data = TWData::from_ptr_as_ref(data)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let iv = TWData::from_ptr_as_ref(iv)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let key = TWData::from_ptr_as_ref(key)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    aes_cbc_encrypt_192(data, iv, key, padding_mode)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCbcDecrypt192)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_cbc_decrypt_192(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
    padding_mode: u32,
) -> NullableMut<TWData> {
    let padding_mode = try_or_else!(PaddingMode::try_from(padding_mode), std::ptr::null_mut);
    let data = TWData::from_ptr_as_ref(data)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let iv = TWData::from_ptr_as_ref(iv)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let key = TWData::from_ptr_as_ref(key)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    aes_cbc_decrypt_192(data, iv, key, padding_mode)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCbcEncrypt256)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_cbc_encrypt_256(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
    padding_mode: u32,
) -> NullableMut<TWData> {
    let padding_mode = try_or_else!(PaddingMode::try_from(padding_mode), std::ptr::null_mut);
    let data = TWData::from_ptr_as_ref(data)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let iv = TWData::from_ptr_as_ref(iv)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let key = TWData::from_ptr_as_ref(key)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    aes_cbc_encrypt_256(data, iv, key, padding_mode)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCbcDecrypt256)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_cbc_decrypt_256(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
    padding_mode: u32,
) -> NullableMut<TWData> {
    let padding_mode = try_or_else!(PaddingMode::try_from(padding_mode), std::ptr::null_mut);
    let data = TWData::from_ptr_as_ref(data)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let iv = TWData::from_ptr_as_ref(iv)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let key = TWData::from_ptr_as_ref(key)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    aes_cbc_decrypt_256(data, iv, key, padding_mode)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCbcEncrypt)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_cbc_encrypt(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
    padding_mode: u32,
) -> NullableMut<TWData> {
    let padding_mode = try_or_else!(PaddingMode::try_from(padding_mode), std::ptr::null_mut);
    let data = TWData::from_ptr_as_ref(data)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let iv = TWData::from_ptr_as_ref(iv)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let key = TWData::from_ptr_as_ref(key)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    aes_cbc_encrypt(data, iv, key, padding_mode)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCbcDecrypt)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_cbc_decrypt(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
    padding_mode: u32,
) -> NullableMut<TWData> {
    let padding_mode = try_or_else!(PaddingMode::try_from(padding_mode), std::ptr::null_mut);
    let data = TWData::from_ptr_as_ref(data)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let iv = TWData::from_ptr_as_ref(iv)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let key = TWData::from_ptr_as_ref(key)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    aes_cbc_decrypt(data, iv, key, padding_mode)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
