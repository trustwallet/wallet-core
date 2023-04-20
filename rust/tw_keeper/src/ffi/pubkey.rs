// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ffi::TWPublicKeyType;
use crate::{secp256k1, Error};
use tw_hash::H256;
use tw_memory::ffi::c_byte_array::CByteArray;
use tw_memory::ffi::c_byte_array_ref::CByteArrayRef;
use tw_memory::ffi::RawPtrTrait;
use tw_utils::{try_or_else, try_or_false};

pub struct TWPublicKey {
    bytes: Vec<u8>,
    ty: TWPublicKeyType,
}

impl RawPtrTrait for TWPublicKey {}

impl TWPublicKey {
    pub fn new(bytes: Vec<u8>, ty: TWPublicKeyType) -> Result<TWPublicKey, Error> {
        if !TWPublicKey::is_valid(&bytes, ty) {
            return Err(Error::InvalidPublicKey);
        }
        Ok(TWPublicKey { bytes, ty })
    }

    pub fn new_unchecked(bytes: Vec<u8>, ty: TWPublicKeyType) -> TWPublicKey {
        TWPublicKey { bytes, ty }
    }

    pub fn is_valid(bytes: &[u8], ty: TWPublicKeyType) -> bool {
        match ty {
            TWPublicKeyType::Secp256k1 => {
                secp256k1::PublicKey::COMPRESSED == bytes.len()
                    && secp256k1::PublicKey::try_from(bytes).is_ok()
            },
            TWPublicKeyType::Secp256k1Extended => {
                secp256k1::PublicKey::UNCOMPRESSED == bytes.len()
                    && secp256k1::PublicKey::try_from(bytes).is_ok()
            },
        }
    }

    pub fn verify(&self, sig: &[u8], hash: &[u8]) -> bool {
        match self.ty {
            TWPublicKeyType::Secp256k1 | TWPublicKeyType::Secp256k1Extended => {
                let verify_sig = try_or_false!(secp256k1::VerifySignature::from_bytes(sig));
                let hash = try_or_false!(H256::try_from(hash));
                let pubkey = try_or_false!(secp256k1::PublicKey::try_from(self.bytes.as_slice()));
                pubkey.verify(verify_sig, hash)
            },
        }
    }
}

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
    CByteArray::new(public.bytes.clone())
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
