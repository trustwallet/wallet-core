// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::crypto_aes_ctr::{
    aes_ctr_decrypt, aes_ctr_decrypt_128, aes_ctr_decrypt_192, aes_ctr_decrypt_256,
    aes_ctr_encrypt, aes_ctr_encrypt_128, aes_ctr_encrypt_192, aes_ctr_encrypt_256,
};
use aes::cipher::StreamCipherError;
use tw_macros::tw_ffi;
use tw_memory::ffi::{tw_data::TWData, Nonnull, NullableMut, RawPtrTrait};

unsafe fn handle_aes_ctr_operation<F>(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
    operation: F,
) -> NullableMut<TWData>
where
    F: FnOnce(&[u8], &[u8], &[u8]) -> Result<Vec<u8>, StreamCipherError>,
{
    let data = TWData::from_ptr_as_ref(data)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let iv = TWData::from_ptr_as_ref(iv)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let key = TWData::from_ptr_as_ref(key)
        .map(|data| data.as_slice())
        .unwrap_or_default();

    operation(data, iv, key)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCtrEncrypt128)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_ctr_encrypt_128(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_encrypt_128)
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCtrDecrypt128)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_ctr_decrypt_128(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_decrypt_128)
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCtrEncrypt192)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_ctr_encrypt_192(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_encrypt_192)
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCtrDecrypt192)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_ctr_decrypt_192(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_decrypt_192)
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCtrEncrypt256)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_ctr_encrypt_256(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_encrypt_256)
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCtrDecrypt256)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_ctr_decrypt_256(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_decrypt_256)
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCtrEncrypt)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_ctr_encrypt(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_encrypt)
}

#[tw_ffi(ty = static_function, class = TWCrypto, name = AesCtrDecrypt)]
#[no_mangle]
pub unsafe extern "C" fn crypto_aes_ctr_decrypt(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_decrypt)
}
