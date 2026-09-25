// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::ffi::crypto_box::public_key::TWCryptoBoxPublicKey;
use crate::ffi::crypto_box::secret_key::TWCryptoBoxSecretKey;
use crate::nacl_crypto_box::CryptoBox;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;

pub mod public_key;
pub mod secret_key;

/// Encrypts message using `my_secret` and `other_pubkey`.
/// The output will have a randomly generated nonce prepended to it.
/// The output will be Overhead + 24 bytes longer than the original.
///
/// \param my_secret *non-null* pointer to my secret key.
/// \param other_pubkey *non-null* pointer to other's public key.
/// \param message *non-null* pointer to the message to be encrypted.
/// \return *nullable* pointer to the encrypted message with randomly generated nonce prepended to it.
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_encrypt_easy(
    my_secret: *const TWCryptoBoxSecretKey,
    other_pubkey: *const TWCryptoBoxPublicKey,
    message: *const TWData,
) -> *mut TWData {
    let my_secret = try_or_else!(
        TWCryptoBoxSecretKey::from_ptr_as_ref(my_secret),
        std::ptr::null_mut
    );
    let other_pubkey = try_or_else!(
        TWCryptoBoxPublicKey::from_ptr_as_ref(other_pubkey),
        std::ptr::null_mut
    );
    let message = try_or_else!(TWData::from_ptr_as_ref(message), std::ptr::null_mut);

    CryptoBox::encrypt_easy(&my_secret.0, &other_pubkey.0, message.as_slice())
        .map(TWData::from)
        .map(TWData::into_ptr)
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Decrypts box produced by `TWCryptoBoxEncryptEasy`.
/// We assume a 24-byte nonce is prepended to the encrypted text in box.
///
/// \param my_secret *non-null* pointer to my secret key.
/// \param other_pubkey *non-null* pointer to other's public key.
/// \param encrypted *non-null* pointer to the encrypted message with nonce prepended to it.
/// \return *nullable* pointer to the decrypted message.
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_decrypt_easy(
    my_secret: *const TWCryptoBoxSecretKey,
    other_pubkey: *const TWCryptoBoxPublicKey,
    encrypted: *const TWData,
) -> *mut TWData {
    let my_secret = try_or_else!(
        TWCryptoBoxSecretKey::from_ptr_as_ref(my_secret),
        std::ptr::null_mut
    );
    let other_pubkey = try_or_else!(
        TWCryptoBoxPublicKey::from_ptr_as_ref(other_pubkey),
        std::ptr::null_mut
    );
    let encrypted = try_or_else!(TWData::from_ptr_as_ref(encrypted), std::ptr::null_mut);

    CryptoBox::decrypt_easy(&my_secret.0, &other_pubkey.0, encrypted.as_slice())
        .map(TWData::from)
        .map(TWData::into_ptr)
        .unwrap_or_else(|_| std::ptr::null_mut())
}
