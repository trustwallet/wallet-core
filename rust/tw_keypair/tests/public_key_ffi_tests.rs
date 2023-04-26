// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_encoding::hex;
use tw_hash::sha3::keccak256;
use tw_hash::{H256, H520};
use tw_keypair::ffi::pubkey::{
    tw_public_key_create_with_data, tw_public_key_delete, tw_public_key_verify, TWPublicKey,
};
use tw_keypair::tw::PublicKeyType;
use tw_memory::ffi::c_byte_array::CByteArray;

struct TWPublicKeyHelper {
    ptr: *mut TWPublicKey,
}

impl TWPublicKeyHelper {
    fn with_bytes<T: Into<Vec<u8>>>(bytes: T, ty: PublicKeyType) -> TWPublicKeyHelper {
        let public_key_raw = CByteArray::from(bytes.into());
        let ptr = unsafe {
            tw_public_key_create_with_data(public_key_raw.data(), public_key_raw.size(), ty as u32)
        };
        TWPublicKeyHelper { ptr }
    }

    fn with_hex(hex: &'static str, ty: PublicKeyType) -> TWPublicKeyHelper {
        let priv_key_data = hex::decode(hex).unwrap();
        TWPublicKeyHelper::with_bytes(priv_key_data, ty)
    }
}

impl Drop for TWPublicKeyHelper {
    fn drop(&mut self) {
        if self.ptr.is_null() {
            return;
        }
        unsafe { tw_public_key_delete(self.ptr) };
    }
}

#[test]
fn test_tw_public_key_create_by_type() {
    let tw_public = TWPublicKeyHelper::with_hex(
        "02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992",
        PublicKeyType::Secp256k1,
    );
    assert!(!tw_public.ptr.is_null());

    // Compressed pubkey with '03' prefix.
    let tw_public = TWPublicKeyHelper::with_hex(
        "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1",
        PublicKeyType::Secp256k1,
    );
    assert!(!tw_public.ptr.is_null());

    let tw_public = TWPublicKeyHelper::with_hex(
        "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91",
        PublicKeyType::Secp256k1Extended,
    );
    assert!(!tw_public.ptr.is_null());

    // Pass an extended pubkey, but Secp256k1 type.
    let tw_public = TWPublicKeyHelper::with_hex(
        "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91",
        PublicKeyType::Secp256k1,
    );
    assert!(tw_public.ptr.is_null());

    // Pass a compressed pubkey, but Secp256k1Extended type.
    let tw_public = TWPublicKeyHelper::with_hex(
        "02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992",
        PublicKeyType::Secp256k1Extended,
    );
    assert!(tw_public.ptr.is_null());
}

#[test]
fn test_tw_public_key_delete_null() {
    unsafe { tw_public_key_delete(std::ptr::null_mut()) };
}

#[test]
fn test_tw_public_key_verify() {
    let tw_public = TWPublicKeyHelper::with_hex(
        "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1",
        PublicKeyType::Secp256k1,
    );
    assert!(!tw_public.ptr.is_null());

    let signature_bytes  = H520::from("8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901");
    let signature_raw = CByteArray::from(signature_bytes.into_vec());
    let hash = H256::try_from(keccak256(b"hello").as_slice()).unwrap();
    let hash_raw = CByteArray::from(hash.to_vec());

    let is_valid = unsafe {
        tw_public_key_verify(
            tw_public.ptr,
            signature_raw.data(),
            signature_raw.size(),
            hash_raw.data(),
            hash_raw.size(),
        )
    };
    assert!(is_valid);
}
