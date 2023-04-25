// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_encoding::hex;
use tw_hash::{H256, H520};
use tw_keypair::ffi::privkey::{
    tw_private_key_create_with_data, tw_private_key_delete, tw_private_key_get_public_key_by_type,
    tw_private_key_is_valid, tw_private_key_sign, TWPrivateKey,
};
use tw_keypair::ffi::pubkey::{tw_public_key_data, tw_public_key_delete};
use tw_keypair::tw::{Curve, PublicKeyType};
use tw_memory::ffi::c_byte_array::CByteArray;

struct TWPrivateKeyHelper {
    ptr: *mut TWPrivateKey,
}

impl TWPrivateKeyHelper {
    fn with_bytes<T: Into<Vec<u8>>>(bytes: T) -> TWPrivateKeyHelper {
        let priv_key_raw = CByteArray::from(bytes.into());
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

    // Invalid hex.
    let tw_privkey = TWPrivateKeyHelper::with_bytes(*b"123");
    assert!(tw_privkey.ptr.is_null());

    // Zero private key.
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
    let hash_raw = CByteArray::from(hash.to_vec());
    let actual = unsafe {
        tw_private_key_sign(
            tw_privkey.ptr,
            hash_raw.data(),
            hash_raw.size(),
            Curve::Secp256k1 as u32,
        )
        .into_vec()
    };
    let expected = H520::from("8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901");
    assert_eq!(actual, expected.into_vec());
}

#[test]
fn test_tw_private_key_sign_invalid_hash() {
    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
    );
    let hash = hex::decode("0xf86a808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000808080").unwrap();
    let hash_raw = CByteArray::from(hash);
    let actual = unsafe {
        tw_private_key_sign(
            tw_privkey.ptr,
            hash_raw.data(),
            hash_raw.size(),
            Curve::Secp256k1 as u32,
        )
        .into_vec()
    };
    assert!(actual.is_empty());
}

#[test]
fn test_tw_private_key_sign_null_hash() {
    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
    );
    let actual = unsafe {
        tw_private_key_sign(tw_privkey.ptr, std::ptr::null(), 0, Curve::Secp256k1 as u32).into_vec()
    };
    assert!(actual.is_empty());
}

#[test]
fn test_tw_private_key_get_public_key_by_type() {
    #[track_caller]
    fn test_get_public_key_data_hex(tw_privkey: &TWPrivateKeyHelper, ty: PublicKeyType) -> String {
        let tw_pubkey = unsafe { tw_private_key_get_public_key_by_type(tw_privkey.ptr, ty as u32) };
        assert!(!tw_pubkey.is_null());

        let actual = unsafe { tw_public_key_data(tw_pubkey).into_vec() };
        unsafe { tw_public_key_delete(tw_pubkey) };
        hex::encode(actual, false)
    }

    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
    );
    assert!(!tw_privkey.ptr.is_null());

    // secp256k1 compressed
    let actual = test_get_public_key_data_hex(&tw_privkey, PublicKeyType::Secp256k1);
    assert_eq!(
        actual,
        "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1"
    );

    // secp256k1 uncompressed
    let actual = test_get_public_key_data_hex(&tw_privkey, PublicKeyType::Secp256k1Extended);
    assert_eq!(actual, "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91");
}

#[test]
fn test_tw_private_key_is_valid() {
    fn is_valid(privkey_bytes: Vec<u8>) -> bool {
        let privkey_raw = CByteArray::from(privkey_bytes);
        unsafe {
            tw_private_key_is_valid(
                privkey_raw.data(),
                privkey_raw.size(),
                Curve::Secp256k1 as u32,
            )
        }
    }

    // Non-zero private key.
    let privkey_bytes =
        H256::from("0000000000000000000000000000000000000000000000000000000000000001");
    assert!(is_valid(privkey_bytes.into_vec()));

    // Cardano private key is not supported yet.
    let privkey_bytes =
        hex::decode("089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e").unwrap();
    assert!(!is_valid(privkey_bytes));

    // Zero private key.
    let privkey_bytes =
        H256::from("0000000000000000000000000000000000000000000000000000000000000000");
    assert!(!is_valid(privkey_bytes.into_vec()));
}
