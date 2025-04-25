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
use aes::cipher::StreamCipherError;
use tw_macros::tw_ffi;
use tw_memory::ffi::{tw_data::TWData, Nonnull, NullableMut, RawPtrTrait};
use tw_misc::try_or_else;

unsafe fn handle_aes_cbc_operation<F>(
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    key: Nonnull<TWData>,
    padding_mode: u32,
    operation: F,
) -> NullableMut<TWData>
where
    F: FnOnce(&[u8], &[u8], &[u8], PaddingMode) -> Result<Vec<u8>, StreamCipherError>,
{
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

    operation(data, iv, key, padding_mode)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Encrypts a block of Data using AES in Cipher Block Chaining (CBC) mode with 128-bit key.
///
/// \param key encryption key Data, must be 16 bytes long.
/// \param data Data to encrypt.
/// \param iv initialization vector.
/// \param mode padding mode.
/// \return encrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = EncryptCBC128)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_encrypt_cbc_128(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    mode: u32,
) -> NullableMut<TWData> {
    handle_aes_cbc_operation(data, iv, key, mode, aes_cbc_encrypt_128)
}

/// Decrypts a block of Data using AES in Cipher Block Chaining (CBC) mode with 128-bit key.
///
/// \param key decryption key Data, must be 16 bytes long.
/// \param data Data to decrypt.
/// \param iv initialization vector.
/// \param mode padding mode.
/// \return decrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = DecryptCBC128)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_decrypt_cbc_128(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    mode: u32,
) -> NullableMut<TWData> {
    handle_aes_cbc_operation(data, iv, key, mode, aes_cbc_decrypt_128)
}

/// Encrypts a block of Data using AES in Cipher Block Chaining (CBC) mode with 192-bit key.
///
/// \param key encryption key Data, must be 24 bytes long.
/// \param data Data to encrypt.
/// \param iv initialization vector.
/// \param mode padding mode.
/// \return encrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = EncryptCBC192)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_encrypt_cbc_192(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    mode: u32,
) -> NullableMut<TWData> {
    handle_aes_cbc_operation(data, iv, key, mode, aes_cbc_encrypt_192)
}

/// Decrypts a block of Data using AES in Cipher Block Chaining (CBC) mode with 192-bit key.
///
/// \param key decryption key Data, must be 24 bytes long.
/// \param data Data to decrypt.
/// \param iv initialization vector.
/// \param mode padding mode.
/// \return decrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = DecryptCBC192)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_decrypt_cbc_192(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    mode: u32,
) -> NullableMut<TWData> {
    handle_aes_cbc_operation(data, iv, key, mode, aes_cbc_decrypt_192)
}

/// Encrypts a block of Data using AES in Cipher Block Chaining (CBC) mode with 256-bit key.
///
/// \param key encryption key Data, must be 32 bytes long.
/// \param data Data to encrypt.
/// \param iv initialization vector.
/// \param mode padding mode.
/// \return encrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = EncryptCBC256)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_encrypt_cbc_256(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    mode: u32,
) -> NullableMut<TWData> {
    handle_aes_cbc_operation(data, iv, key, mode, aes_cbc_encrypt_256)
}

/// Decrypts a block of Data using AES in Cipher Block Chaining (CBC) mode with 256-bit key.
///
/// \param key decryption key Data, must be 32 bytes long.
/// \param data Data to decrypt.
/// \param iv initialization vector.
/// \param mode padding mode.
/// \return decrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = DecryptCBC256)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_decrypt_cbc_256(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    mode: u32,
) -> NullableMut<TWData> {
    handle_aes_cbc_operation(data, iv, key, mode, aes_cbc_decrypt_256)
}

/// Encrypts a block of Data using AES in Cipher Block Chaining (CBC) mode.
///
/// \param key encryption key Data, must be 16, 24, or 32 bytes long.
/// \param data Data to encrypt.
/// \param iv initialization vector.
/// \param mode padding mode.
/// \return encrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = EncryptCBC)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_encrypt_cbc(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    mode: u32,
) -> NullableMut<TWData> {
    handle_aes_cbc_operation(data, iv, key, mode, aes_cbc_encrypt)
}

/// Decrypts a block of data using AES in Cipher Block Chaining (CBC) mode.
///
/// \param key decryption key Data, must be 16, 24, or 32 bytes long.
/// \param data Data to decrypt.
/// \param iv initialization vector Data.
/// \param mode padding mode.
/// \return decrypted Data.
#[tw_ffi(ty = static_function, class = TWAES, name = DecryptCBC)]
#[no_mangle]
pub unsafe extern "C" fn tw_aes_decrypt_cbc(
    key: Nonnull<TWData>,
    data: Nonnull<TWData>,
    iv: Nonnull<TWData>,
    mode: u32,
) -> NullableMut<TWData> {
    handle_aes_cbc_operation(data, iv, key, mode, aes_cbc_decrypt)
}
