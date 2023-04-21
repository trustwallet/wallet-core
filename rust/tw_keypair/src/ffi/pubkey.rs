// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ffi::TWPublicKeyType;
use crate::tw::TWPublicKey;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::RawPtrTrait;
use tw_utils::{try_or_else, try_or_false};

#[no_mangle]
pub unsafe extern "C" fn tw_public_key_create_with_data(
    input: *const u8,
    input_len: usize,
    ty: u32,
) -> *mut TWPublicKey {
    let bytes_ref = CByteArrayRef::new(input, input_len);
    let bytes = bytes_ref.to_vec().unwrap_or_default();

    let ty = try_or_else!(TWPublicKeyType::from_raw(ty), std::ptr::null_mut);
    TWPublicKey::new(bytes, ty)
        .map(TWPublicKey::into_ptr)
        .unwrap_or_else(|_| std::ptr::null_mut())
}

#[no_mangle]
pub unsafe extern "C" fn tw_public_key_delete(key: *mut TWPublicKey) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWPublicKey::from_ptr(key);
}

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
    public.verify(sig, msg)
}

#[no_mangle]
pub unsafe extern "C" fn tw_public_key_data(key: *mut TWPublicKey) -> CByteArray {
    let public = try_or_else!(TWPublicKey::from_ptr_as_ref(key), CByteArray::empty);
    CByteArray::new(public.to_bytes())
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
