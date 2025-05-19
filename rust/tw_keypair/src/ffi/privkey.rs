// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::ffi::pubkey::TWPublicKey;
use crate::tw::{Curve, PrivateKey, PublicKeyType};
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::{try_or_else, try_or_false};

pub struct TWPrivateKey(pub(crate) PrivateKey);

impl RawPtrTrait for TWPrivateKey {}

impl AsRef<PrivateKey> for TWPrivateKey {
    fn as_ref(&self) -> &PrivateKey {
        &self.0
    }
}

/// Create a private key with the given block of data.
///
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \note Should be deleted with \tw_private_key_delete.
/// \return Nullable pointer to Private Key.
#[no_mangle]
pub unsafe extern "C" fn tw_private_key_create_with_data(
    input: *const u8,
    input_len: usize,
) -> *mut TWPrivateKey {
    let bytes_ref = CByteArrayRef::new(input, input_len);
    let bytes = try_or_else!(bytes_ref.to_vec(), std::ptr::null_mut);

    PrivateKey::new(bytes)
        .map(|private| TWPrivateKey(private).into_ptr())
        // Return null if the private key is invalid.
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Delete the given private key.
///
/// \param key *non-null* pointer to private key.
#[no_mangle]
pub unsafe extern "C" fn tw_private_key_delete(key: *mut TWPrivateKey) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWPrivateKey::from_ptr(key);
}

/// Returns the raw pointer to the underlying bytes of the private key.
///
/// \param data A non-null valid block of private key
/// \return the raw pointer to the contents of private key
#[no_mangle]
pub unsafe extern "C" fn tw_private_key_bytes(data: *const TWPrivateKey) -> *const u8 {
    TWPrivateKey::from_ptr_as_ref(data)
        .map(|data| data.0.bytes().as_ptr())
        .unwrap_or_else(std::ptr::null)
}

/// Returns the size in bytes.
///
/// \param data A non-null valid block of private key
/// \return the size of the given block of private key
#[no_mangle]
pub unsafe extern "C" fn tw_private_key_size(data: *const TWPrivateKey) -> usize {
    TWPrivateKey::from_ptr_as_ref(data)
        .map(|data| data.0.bytes().len())
        .unwrap_or_default()
}

/// Determines if the given private key is valid or not.
///
/// \param key *non-null* byte array.
/// \param key_len the length of the `key` array.
/// \param curve Elliptic curve of the private key.
/// \return true if the private key is valid, false otherwise.
#[no_mangle]
pub unsafe extern "C" fn tw_private_key_is_valid(
    key: *const u8,
    key_len: usize,
    curve: u32,
) -> bool {
    let curve = try_or_false!(Curve::from_raw(curve));
    let priv_key_slice = try_or_false!(CByteArrayRef::new(key, key_len).as_slice());
    PrivateKey::is_valid(priv_key_slice, curve)
}

/// Signs a digest using ECDSA and given curve.
///
/// \param key *non-null* pointer to a Private key
/// \param message *non-null* byte array.
/// \param message_len the length of the `input` array.
/// \param curve Elliptic curve.
/// \return Signature as a C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn tw_private_key_sign(
    key: *mut TWPrivateKey,
    message: *const u8,
    message_len: usize,
    curve: u32,
) -> CByteArray {
    let curve = try_or_else!(Curve::from_raw(curve), CByteArray::default);
    let private = try_or_else!(TWPrivateKey::from_ptr_as_ref(key), CByteArray::default);
    let message_to_sign = try_or_else!(
        CByteArrayRef::new(message, message_len).as_slice(),
        CByteArray::default
    );

    // Return an empty signature if an error occurs.
    let sig = private.0.sign(message_to_sign, curve).unwrap_or_default();
    CByteArray::from(sig)
}

/// Returns the public key associated with the given pubkeyType and privateKey
///
/// \param key *non-null* pointer to the private key.
/// \param pubkey_type type of the public key to return.
/// \return *non-null* pointer to the corresponding public key.
#[no_mangle]
pub unsafe extern "C" fn tw_private_key_get_public_key_by_type(
    key: *mut TWPrivateKey,
    pubkey_type: u32,
) -> *mut TWPublicKey {
    let ty = try_or_else!(PublicKeyType::from_raw(pubkey_type), std::ptr::null_mut);
    let private = try_or_else!(TWPrivateKey::from_ptr_as_ref(key), std::ptr::null_mut);
    private
        .0
        .get_public_key_by_type(ty)
        .map(|public| TWPublicKey(public).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

// #[no_mangle]
// pub unsafe extern "C" fn tw_private_key_get_shared_key(
//     key: *mut TWPrivateKey,
//     hash: *const u8,
//     hash_len: usize,
//     curve: u32,
// ) -> CByteArray {
//     let curve = match Curve::from_raw(curve) {
//         Some(curve) => curve,
//         None => return CByteArray::empty(),
//     };
//     let private = try_or_else!(TWPrivateKey::from_ptr_as_ref(key), CByteArray::empty);
//
//     let hash = CByteArrayRef::new(hash, hash_len);
//     let hash_to_sign = match hash.as_slice() {
//         Some(hash) => hash,
//         None => return CByteArray::empty(),
//     };
//
//     // Return an empty signature if an error occurs.
//     let sig = private.sign(hash_to_sign, curve).unwrap_or_default();
//     CByteArray::from(sig)
// }
