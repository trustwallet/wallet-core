// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::tw::{PublicKey, PublicKeyType};
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::{try_or_else, try_or_false};

pub struct TWPublicKey(pub(crate) PublicKey);

impl AsRef<PublicKey> for TWPublicKey {
    fn as_ref(&self) -> &PublicKey {
        &self.0
    }
}

impl RawPtrTrait for TWPublicKey {}

/// Create a public key with the given block of data and specified public key type.
///
/// \param input *non-null* byte array.
/// \param input_len the length of the `input` array.
/// \param ty type of the public key.
/// \note Should be deleted with \tw_public_key_delete.
/// \return Nullable pointer to the public key.
#[no_mangle]
pub unsafe extern "C" fn tw_public_key_create_with_data(
    input: *const u8,
    input_len: usize,
    ty: u32,
) -> *mut TWPublicKey {
    let bytes_ref = CByteArrayRef::new(input, input_len);
    let bytes = bytes_ref.to_vec();
    let ty = try_or_else!(PublicKeyType::from_raw(ty), std::ptr::null_mut);
    PublicKey::new(bytes, ty)
        .map(|public| TWPublicKey(public).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Determines if the given public key is valid or not.
///
/// \param key *non-null* byte array.
/// \param key_len the length of the `key` array.
/// \param curve Eliptic curve of the public key.
/// \return true if the public key is valid, false otherwise.
#[no_mangle]
pub unsafe extern "C" fn tw_public_key_is_valid(
    key: *const u8,
    key_len: usize,
    pubkey_type: u32,
) -> bool {
    let pubkey_type = try_or_false!(PublicKeyType::from_raw(pubkey_type));
    let pub_key_bytes = try_or_false!(CByteArrayRef::new(key, key_len).to_vec());
    PublicKey::is_valid(pub_key_bytes, pubkey_type)
}

/// Delete the given public key.
///
/// \param key *non-null* pointer to public key.
#[no_mangle]
pub unsafe extern "C" fn tw_public_key_delete(key: *mut TWPublicKey) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWPublicKey::from_ptr(key);
}

/// Verify the validity of a signature and a message using the given public key.
///
/// \param key *non-null* pointer to a Public key.
/// \param sig *non-null* pointer to a block of data corresponding to the signature.
/// \param sig_len the length of the `sig` array.
/// \param msg *non-null* pointer to a block of data corresponding to the message.
/// \param msg_len the length of the `msg` array.
/// \return true if the signature and the message belongs to the given public key, otherwise false.
#[no_mangle]
pub unsafe extern "C" fn tw_public_key_verify(
    key: *mut TWPublicKey,
    sig: *const u8,
    sig_len: usize,
    msg: *const u8,
    msg_len: usize,
) -> bool {
    let public = try_or_false!(TWPublicKey::from_ptr_as_ref(key));
    let sig = CByteArrayRef::new(sig, sig_len).as_slice();
    let msg = CByteArrayRef::new(msg, msg_len).as_slice();
    public.0.verify(sig, msg)
}

/// Returns the raw data of a given public-key.
///
/// \param key *non-null* pointer to a public key.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn tw_public_key_data(key: *mut TWPublicKey) -> CByteArray {
    let public = try_or_else!(TWPublicKey::from_ptr_as_ref(key), CByteArray::default);
    CByteArray::from(public.0.to_bytes())
}

/// Returns the type of a given public-key.
///
/// \param key *non-null* pointer to a public key.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn tw_public_key_type(key: *mut TWPublicKey) -> u32 {
    let public = try_or_else!(TWPublicKey::from_ptr_as_ref(key), || 0);
    public.0.public_key_type() as u32
}

/// Returns the compressed data of a given public-key.
///
/// \param key *non-null* pointer to a public key.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn tw_public_key_compressed(key: *mut TWPublicKey) -> *mut TWPublicKey {
    let public = try_or_else!(TWPublicKey::from_ptr_as_ref(key), std::ptr::null_mut);
    public
        .0
        .compressed()
        .map(|public| TWPublicKey(public).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Returns the extended data of a given public-key.
///
/// \param key *non-null* pointer to a public key.
/// \return C-compatible result with a C-compatible byte array.
#[no_mangle]
pub unsafe extern "C" fn tw_public_key_extended(key: *mut TWPublicKey) -> *mut TWPublicKey {
    let public = try_or_else!(TWPublicKey::from_ptr_as_ref(key), std::ptr::null_mut);
    public
        .0
        .extended()
        .map(|public| TWPublicKey(public).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Recover a public key from a signature and a message.
///
/// \param sig *non-null* pointer to a block of data corresponding to the signature.
/// \param sig_len the length of the `sig` array.
/// \param msg *non-null* pointer to a block of data corresponding to the message.
/// \param msg_len the length of the `msg` array.
/// \return C-compatible result with a C-compatible pointer to the public key.
#[no_mangle]
pub unsafe extern "C" fn tw_public_key_recover_from_signature(
    sig: *const u8,
    sig_len: usize,
    msg: *const u8,
    msg_len: usize,
    rec_id: u8,
) -> *mut TWPublicKey {
    let sig = try_or_else!(
        CByteArrayRef::new(sig, sig_len).as_slice(),
        std::ptr::null_mut
    );
    let msg = try_or_else!(
        CByteArrayRef::new(msg, msg_len).as_slice(),
        std::ptr::null_mut
    );
    PublicKey::recover_from_signature(sig, msg, rec_id)
        .map(|public| TWPublicKey(public).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Verify a signature as DER-encoded ECDSA signature.
///
/// \param key *non-null* pointer to a public key.
/// \param sig *non-null* pointer to a block of data corresponding to the signature.
/// \param sig_len the length of the `sig` array.
/// \param msg *non-null* pointer to a block of data corresponding to the message.
/// \param msg_len the length of the `msg` array.
/// \return true if the signature and the message belongs to the given public key, otherwise false.
#[no_mangle]
pub unsafe extern "C" fn tw_public_key_verify_as_der(
    key: *mut TWPublicKey,
    sig: *const u8,
    sig_len: usize,
    msg: *const u8,
    msg_len: usize,
) -> bool {
    let public = try_or_false!(TWPublicKey::from_ptr_as_ref(key));
    let sig = try_or_false!(CByteArrayRef::new(sig, sig_len).as_slice());
    let msg = try_or_false!(CByteArrayRef::new(msg, msg_len).as_slice());
    public.0.verify_as_der(sig, msg)
}
