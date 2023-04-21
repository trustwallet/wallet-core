// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ffi::{TWCurve, TWPublicKeyType};
use crate::tw::{TWPrivateKey, TWPublicKey};
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::RawPtrTrait;
use tw_utils::{try_or_else, try_or_false};

#[no_mangle]
pub unsafe extern "C" fn tw_private_key_create_with_data(
    input: *const u8,
    input_len: usize,
) -> *mut TWPrivateKey {
    let bytes_ref = CByteArrayRef::new(input, input_len);
    let bytes = try_or_else!(bytes_ref.to_vec(), std::ptr::null_mut);

    TWPrivateKey::new(bytes)
        .map(TWPrivateKey::into_ptr)
        // Return null if the private key is invalid.
        .unwrap_or_else(|_| std::ptr::null_mut())
}

#[no_mangle]
pub unsafe extern "C" fn tw_private_key_delete(key: *mut TWPrivateKey) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWPrivateKey::from_ptr(key);
}

#[no_mangle]
pub unsafe extern "C" fn tw_private_key_is_valid(
    key: *const u8,
    key_len: usize,
    curve: u32,
) -> bool {
    let curve = try_or_false!(TWCurve::from_raw(curve));
    let priv_key_slice = try_or_false!(CByteArrayRef::new(key, key_len).as_slice());
    TWPrivateKey::is_valid(priv_key_slice, curve)
}

#[no_mangle]
pub unsafe extern "C" fn tw_private_key_sign(
    key: *mut TWPrivateKey,
    hash: *const u8,
    hash_len: usize,
    curve: u32,
) -> CByteArray {
    let curve = match TWCurve::from_raw(curve) {
        Some(curve) => curve,
        None => return CByteArray::empty(),
    };
    let private = try_or_else!(TWPrivateKey::from_ptr_as_ref(key), CByteArray::empty);
    let hash_to_sign = try_or_else!(
        CByteArrayRef::new(hash, hash_len).as_slice(),
        CByteArray::empty
    );

    // Return an empty signature if an error occurs.
    let sig = private.sign(hash_to_sign, curve).unwrap_or_default();
    CByteArray::new(sig)
}

#[no_mangle]
pub unsafe extern "C" fn tw_private_key_get_public_key_by_type(
    key: *mut TWPrivateKey,
    pubkey_type: u32,
) -> *mut TWPublicKey {
    let ty = try_or_else!(TWPublicKeyType::from_raw(pubkey_type), std::ptr::null_mut);
    let private = try_or_else!(TWPrivateKey::from_ptr_as_ref(key), std::ptr::null_mut);
    private
        .get_public_key_by_type(ty)
        .map(TWPublicKey::into_ptr)
        .unwrap_or_else(|_| std::ptr::null_mut())
}

// #[no_mangle]
// pub unsafe extern "C" fn tw_private_key_get_shared_key(
//     key: *mut TWPrivateKey,
//     hash: *const u8,
//     hash_len: usize,
//     curve: u32,
// ) -> CByteArray {
//     let curve = match TWCurve::from_raw(curve) {
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
//     CByteArray::new(sig)
// }
