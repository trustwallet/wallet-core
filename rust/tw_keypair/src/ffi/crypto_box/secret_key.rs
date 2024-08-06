// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::ffi::crypto_box::public_key::TWCryptoBoxPublicKey;
use crate::nacl_crypto_box::secret_key::SecretKey;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;

/// Secret key used in `crypto_box` cryptography.
pub struct TWCryptoBoxSecretKey(pub(crate) SecretKey);

impl RawPtrTrait for TWCryptoBoxSecretKey {}

/// Create a random secret key.
///
/// \note Should be deleted with \tw_crypto_box_secret_key_delete.
/// \return Nullable pointer to Private Key.
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_secret_key_create() -> *mut TWCryptoBoxSecretKey {
    TWCryptoBoxSecretKey(SecretKey::random()).into_ptr()
}

/// Delete the given secret `key`.
///
/// \param key *non-null* pointer to secret key.
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_secret_key_delete(key: *mut TWCryptoBoxSecretKey) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWCryptoBoxSecretKey::from_ptr(key);
}

/// Returns the public key associated with the given `key`.
///
/// \param key *non-null* pointer to the private key.
/// \return *non-null* pointer to the corresponding public key.
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_secret_key_get_public_key(
    key: *mut TWCryptoBoxSecretKey,
) -> *mut TWCryptoBoxPublicKey {
    let secret = try_or_else!(
        TWCryptoBoxSecretKey::from_ptr_as_ref(key),
        std::ptr::null_mut
    );
    TWCryptoBoxPublicKey(secret.0.public_key()).into_ptr()
}
