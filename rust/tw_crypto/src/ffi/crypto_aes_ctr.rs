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

/// Encrypts a block of data using AES in Counter (CTR) mode with 128-bit key.
///
/// \param key encryption key Data, must be 16 bytes long.
/// \param data Data to encrypt.
/// \param iv initialization vector Data.
/// \return encrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = EncryptCTR128)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_encrypt_ctr_128(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_encrypt_128)
}

/// Decrypts a block of data using AES in Counter (CTR) mode with 128-bit key.
///
/// \param key decryption key Data, must be 16 bytes long.
/// \param data Data to decrypt.
/// \param iv initialization vector Data.
/// \return decrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = DecryptCTR128)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_decrypt_ctr_128(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_decrypt_128)
}

/// Encrypts a block of data using AES in Counter (CTR) mode with 192-bit key.
///
/// \param key encryption key Data, must be 24 bytes long.
/// \param data Data to encrypt.
/// \param iv initialization vector Data.
/// \return encrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = EncryptCTR192)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_encrypt_ctr_192(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_encrypt_192)
}

/// Decrypts a block of data using AES in Counter (CTR) mode with 192-bit key.
///
/// \param key decryption key Data, must be 24 bytes long.
/// \param data Data to decrypt.
/// \param iv initialization vector Data.
/// \return decrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = DecryptCTR192)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_decrypt_ctr_192(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_decrypt_192)
}

/// Encrypts a block of data using AES in Counter (CTR) mode with 256-bit key.
///
/// \param key encryption key Data, must be 32 bytes long.
/// \param data Data to encrypt.
/// \param iv initialization vector Data.
/// \return encrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = EncryptCTR256)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_encrypt_ctr_256(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_encrypt_256)
}

/// Decrypts a block of data using AES in Counter (CTR) mode with 256-bit key.
///
/// \param key decryption key Data, must be 32 bytes long.
/// \param data Data to decrypt.
/// \param iv initialization vector Data.
/// \return decrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = DecryptCTR256)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_decrypt_ctr_256(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_decrypt_256)
}

/// Encrypts a block of data using AES in Counter (CTR) mode.
///
/// \param key encryption key Data, must be 16, 24, or 32 bytes long.
/// \param data Data to encrypt.
/// \param iv initialization vector Data.
/// \return encrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = EncryptCTR)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_encrypt_ctr(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_encrypt)
}

/// Decrypts a block of data using AES in Counter (CTR) mode.
///
/// \param key decryption key Data, must be 16, 24, or 32 bytes long.
/// \param data Data to decrypt.
/// \param iv initialization vector Data.
/// \return decrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = DecryptCTR)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_decrypt_ctr(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
) -> NullableMut<TWData> {
    handle_aes_ctr_operation(data, iv, key, aes_ctr_decrypt)
}
