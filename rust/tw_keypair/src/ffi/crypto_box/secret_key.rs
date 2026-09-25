// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::ffi::crypto_box::public_key::TWCryptoBoxPublicKey;
use crate::nacl_crypto_box::secret_key::SecretKey;
use tw_macros::tw_ffi;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_memory::ffi::{Nonnull, NonnullMut, NullableMut};
use tw_misc::traits::ToBytesZeroizing;
use tw_misc::{try_or_else, try_or_false};

/// Secret key used in `crypto_box` cryptography.
pub struct TWCryptoBoxSecretKey(pub(crate) SecretKey);

impl RawPtrTrait for TWCryptoBoxSecretKey {}

/// Determines if the given secret key is valid or not.
///
/// \param data *non-null* byte array.
/// \return true if the secret key is valid, false otherwise.
#[tw_ffi(ty = static_function, class = TWCryptoBoxSecretKey, name = IsValid)]
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_secret_key_is_valid(data: Nonnull<TWData>) -> bool {
    let bytes_ref = try_or_false!(TWData::from_ptr_as_ref(data));
    SecretKey::try_from(bytes_ref.as_slice()).is_ok()
}

/// Create a random secret key.
///
/// \note Should be deleted with \tw_crypto_box_secret_key_delete.
/// \return Nullable pointer to Private Key.
#[tw_ffi(ty = constructor, class = TWCryptoBoxSecretKey, name = Create)]
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_secret_key_create() -> NonnullMut<TWCryptoBoxSecretKey> {
    TWCryptoBoxSecretKey(SecretKey::random()).into_ptr()
}

/// Create a `crypto_box` secret key with the given block of data.
///
/// \param data *non-null* byte array. Expected to have 32 bytes.
/// \note Should be deleted with \tw_crypto_box_secret_key_delete.
/// \return Nullable pointer to Secret Key.
#[tw_ffi(ty = constructor, class = TWCryptoBoxSecretKey, name = CreateWithData)]
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_secret_key_create_with_data(
    data: Nonnull<TWData>,
) -> NullableMut<TWCryptoBoxSecretKey> {
    let bytes_ref = try_or_else!(TWData::from_ptr_as_ref(data), std::ptr::null_mut);
    let secret = try_or_else!(
        SecretKey::try_from(bytes_ref.as_slice()),
        std::ptr::null_mut
    );
    TWCryptoBoxSecretKey(secret).into_ptr()
}

/// Delete the given secret `key`.
///
/// \param key *non-null* pointer to secret key.
#[tw_ffi(ty = destructor, class = TWCryptoBoxSecretKey, name = Delete)]
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_secret_key_delete(key: NonnullMut<TWCryptoBoxSecretKey>) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWCryptoBoxSecretKey::from_ptr(key);
}

/// Returns the public key associated with the given `key`.
///
/// \param key *non-null* pointer to the private key.
/// \return *non-null* pointer to the corresponding public key.
#[tw_ffi(ty = method, class = TWCryptoBoxSecretKey, name = GetPublicKey)]
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_secret_key_get_public_key(
    key: NonnullMut<TWCryptoBoxSecretKey>,
) -> NonnullMut<TWCryptoBoxPublicKey> {
    let secret = try_or_else!(
        TWCryptoBoxSecretKey::from_ptr_as_ref(key),
        std::ptr::null_mut
    );
    TWCryptoBoxPublicKey(secret.0.public_key()).into_ptr()
}

/// Returns the raw data of a given secret-key.
///
/// \param secret_key *non-null* pointer to a secret key.
/// \return C-compatible result with a C-compatible byte array.
#[tw_ffi(ty = property, class = TWCryptoBoxSecretKey, name = Data)]
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_secret_key_data(
    secret_key: Nonnull<TWCryptoBoxSecretKey>,
) -> NonnullMut<TWData> {
    let secret_ref = try_or_else!(
        TWCryptoBoxSecretKey::from_ptr_as_ref(secret_key),
        std::ptr::null_mut
    );
    TWData::from(secret_ref.0.to_zeroizing_vec().to_vec()).into_ptr()
}
