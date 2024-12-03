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
    let bytes = try_or_else!(bytes_ref.to_vec(), std::ptr::null_mut);
    let ty = try_or_else!(PublicKeyType::from_raw(ty), std::ptr::null_mut);
    PublicKey::new(bytes, ty)
        .map(|public| TWPublicKey(public).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
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
    let sig = try_or_false!(CByteArrayRef::new(sig, sig_len).as_slice());
    let msg = try_or_false!(CByteArrayRef::new(msg, msg_len).as_slice());
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

// #[no_mangle]
// pub unsafe extern "C" fn tw_public_key_is_valid(
//     pubkey: *const u8,
//     pubkey_len: usize,
//     pubkey_type: u32,
// ) -> bool {
//     let ty = match TWPublicKeyType::from_raw(pubkey_type) {
//         Some(ty) => ty,
//         None => return false,
//     };
//
//     let pubkey_slice = match CByteArrayRef::new(pubkey, pubkey_len).as_slice() {
//         Some(pubkey) => pubkey,
//         None => return false,
//     };
//
//     TWPublicKey::is_valid(pubkey_slice, ty)
// }
