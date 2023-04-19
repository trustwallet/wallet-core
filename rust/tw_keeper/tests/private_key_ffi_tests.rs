// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_hash::{H256, H520};
use tw_keeper::ffi::privkey::{
    tw_private_key_create_with_data, tw_private_key_delete, tw_private_key_sign, TWPrivateKey,
};
use tw_keeper::ffi::TWCurve;
use tw_memory::ffi::c_byte_array::CByteArray;

struct TWPrivateKeyHelper {
    ptr: *mut TWPrivateKey,
}

impl TWPrivateKeyHelper {
    fn with_bytes<T: Into<Vec<u8>>>(bytes: T) -> TWPrivateKeyHelper {
        let priv_key_raw = CByteArray::new(bytes.into());
        let ptr =
            unsafe { tw_private_key_create_with_data(priv_key_raw.data(), priv_key_raw.size()) };
        TWPrivateKeyHelper { ptr }
    }

    fn with_hex(hex: &'static str) -> TWPrivateKeyHelper {
        let priv_key_data = H256::from(hex);
        TWPrivateKeyHelper::with_bytes(priv_key_data.into_vec())
    }
}

impl Drop for TWPrivateKeyHelper {
    fn drop(&mut self) {
        if self.ptr.is_null() {
            return;
        }
        unsafe { tw_private_key_delete(self.ptr) };
    }
}

#[test]
fn test_tw_private_key_create() {
    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "ef2cf705af8714b35c0855030f358f2bee356ff3579cea2607b2025d80133c3a",
    );
    assert!(!tw_privkey.ptr.is_null());
}

#[test]
fn test_tw_private_key_create_invalid_hex() {
    let tw_privkey = TWPrivateKeyHelper::with_bytes(*b"123");
    assert!(tw_privkey.ptr.is_null());
}

#[test]
fn test_tw_private_key_create_zero() {
    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "0000000000000000000000000000000000000000000000000000000000000000",
    );
    assert!(tw_privkey.ptr.is_null());
}

#[test]
fn test_tw_private_key_delete_null() {
    unsafe { tw_private_key_create_with_data(std::ptr::null_mut(), 0) };
}

#[test]
fn test_tw_private_key_sign() {
    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
    );
    let hash = H256::from("1c8aff950685c2ed4bc3174f3472287b56d9517b9c948127319a09a7a36deac8");
    let hash_raw = CByteArray::new(hash.to_vec());
    let actual = unsafe {
        tw_private_key_sign(
            tw_privkey.ptr,
            hash_raw.data(),
            hash_raw.size(),
            TWCurve::Secp256k1 as u32,
        )
        .into_vec()
    };
    let expected = H520::from("8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901");
    assert_eq!(actual, expected.into_vec());
}
