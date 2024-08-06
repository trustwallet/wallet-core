// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::nacl_crypto_box::public_key::PublicKey;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::{try_or_else, try_or_false};

/// Public key used in `crypto_box` cryptography.
pub struct TWCryptoBoxPublicKey(pub(crate) PublicKey);

impl RawPtrTrait for TWCryptoBoxPublicKey {}

/// Determines if the given public key is valid or not.
///
/// \param data *non-null* byte array.
/// \return true if the public key is valid, false otherwise.
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_public_key_is_valid(data: *const TWData) -> bool {
    let bytes_ref = try_or_false!(TWData::from_ptr_as_ref(data));
    PublicKey::try_from(bytes_ref.as_slice()).is_ok()
}

/// Create a `crypto_box` public key with the given block of data.
///
/// \param data *non-null* byte array. Expected to have 32 bytes.
/// \note Should be deleted with \tw_crypto_box_public_key_delete.
/// \return Nullable pointer to Public Key.
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_public_key_create_with_data(
    data: *const TWData,
) -> *mut TWCryptoBoxPublicKey {
    let bytes_ref = try_or_else!(TWData::from_ptr_as_ref(data), std::ptr::null_mut);
    let pubkey = try_or_else!(
        PublicKey::try_from(bytes_ref.as_slice()),
        std::ptr::null_mut
    );
    TWCryptoBoxPublicKey(pubkey).into_ptr()
}

/// Delete the given public key.
///
/// \param public_key *non-null* pointer to public key.
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_public_key_delete(public_key: *mut TWCryptoBoxPublicKey) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWCryptoBoxPublicKey::from_ptr(public_key);
}

/// Returns the raw data of a given public-key.
///
/// \param public_key *non-null* pointer to a public key.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn tw_crypto_box_public_key_data(
    public_key: *const TWCryptoBoxPublicKey,
) -> *mut TWData {
    let pubkey_ref = try_or_else!(
        TWCryptoBoxPublicKey::from_ptr_as_ref(public_key),
        std::ptr::null_mut
    );
    TWData::from(pubkey_ref.0.as_slice().to_vec()).into_ptr()
}
