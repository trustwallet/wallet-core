// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex;
use tw_hash::sha2::sha256;
use tw_hash::sha3::keccak256;
use tw_hash::H256;
use tw_keypair::ffi::privkey::{
    tw_private_key_create_with_data, tw_private_key_get_public_key_by_type,
    tw_private_key_is_valid, tw_private_key_sign,
};
use tw_keypair::ffi::pubkey::{tw_public_key_data, tw_public_key_delete};
use tw_keypair::test_utils::tw_private_key_helper::TWPrivateKeyHelper;
use tw_keypair::tw::{Curve, PublicKeyType};
use tw_memory::ffi::c_byte_array::CByteArray;

fn test_sign(curve: Curve, secret: &str, msg: &str, expected_sign: &str) {
    let tw_privkey = TWPrivateKeyHelper::with_hex(secret);
    let msg = hex::decode(msg).unwrap();
    let msg_raw = CByteArray::from(msg);
    let actual = unsafe {
        tw_private_key_sign(
            tw_privkey.ptr(),
            msg_raw.data(),
            msg_raw.size(),
            curve as u32,
        )
        .into_vec()
    };
    let expected = hex::decode(expected_sign).unwrap();
    assert_eq!(actual, expected);
}

#[test]
fn test_tw_private_key_create() {
    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "ef2cf705af8714b35c0855030f358f2bee356ff3579cea2607b2025d80133c3a",
    );
    assert!(!tw_privkey.is_null());

    // Invalid hex.
    let tw_privkey = TWPrivateKeyHelper::with_bytes(*b"123");
    assert!(tw_privkey.is_null());

    // Zero private key.
    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "0000000000000000000000000000000000000000000000000000000000000000",
    );
    assert!(tw_privkey.is_null());
}

#[test]
fn test_tw_private_key_delete_null() {
    unsafe { tw_private_key_create_with_data(std::ptr::null_mut(), 0) };
}

#[test]
fn test_tw_private_key_sign_secp256k1() {
    let secret = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
    let msg = "1c8aff950685c2ed4bc3174f3472287b56d9517b9c948127319a09a7a36deac8";
    let sign = "8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901";
    test_sign(Curve::Secp256k1, secret, msg, sign);
}

#[test]
fn test_tw_private_key_sign_ed25519() {
    let secret = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
    let msg = hex::encode(sha256(b"Hello"), false);
    let sign = "42848abf2641a731e18b8a1fb80eff341a5acebdc56faeccdcbadb960aef775192842fccec344679446daa4d02d264259c8f9aa364164ebe0ebea218581e2e03";
    test_sign(Curve::Ed25519, secret, &msg, sign);
}

#[test]
fn test_tw_private_key_sign_ed25519_blake2b() {
    let secret = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
    let msg = hex::encode(sha256(b"Hello"), false);
    let sign = "5c1473944cd0234ebc5a91b2966b9e707a33b936dadd149417a2e53b6b3fc97bef17b767b1690708c74d7b4c8fe48703fd44a6ef59d4cc5b9f88ba992db0a003";
    test_sign(Curve::Ed25519Blake2bNano, secret, &msg, sign);
}

#[test]
fn test_tw_private_key_sign_nist256p1() {
    let secret = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
    let msg = hex::encode(keccak256(b"hello"), false);
    let sign = "8859e63a0c0cc2fc7f788d7e78406157b288faa6f76f76d37c4cd1534e8d83c468f9fd6ca7dde378df594625dcde98559389569e039282275e3d87c26e36447401";
    test_sign(Curve::Nist256p1, secret, &msg, sign);
}

#[test]
fn test_tw_private_key_sign_curve25519_waves() {
    let secret = "c45d1ba60a5d929d228d1b69a8f91bd256262498e81d32b6411d6dac9a60ed3b";
    let msg = "e6167d";
    let sign = "05db2483b8107187448e7f3d5581e48380a83338d53fe70c59e88b281b995216085518e5d331a2b698f2d0d387529dd94437157df88cb14be8d1925afbb6e80d";
    test_sign(Curve::Curve25519Waves, secret, msg, sign);
}

#[test]
fn test_tw_private_key_sign_ed25519_extended_cardano() {
    let secret = "e8c8c5b2df13f3abed4e6b1609c808e08ff959d7e6fc3d849e3f2880550b574437aa559095324d78459b9bb2da069da32337e1cc5da78f48e1bd084670107f3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26fa\
        e0d152bb611cb9ff34e945e4ff627e6fba81da687a601a879759cd76530b5744424db69a75edd4780a5fbc05d1a3c84ac4166ff8e424808481dd8e77627ce5f5bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276";
    let msg = hex::encode(sha256(b"Hello"), false);
    let sign = "0a8c6e36f9afa324e9065d185cf5df2815c6b997b2e1627e8612ddba8097dcfc325ad6b2317cda2159407463cdd2706af97f299873e940f43f986951f8809108";
    test_sign(Curve::Ed25519ExtendedCardano, secret, &msg, sign);
}

#[test]
fn test_tw_private_key_sign_starkex() {
    let secret = "0139fe4d6f02e666e86a6f58e65060f115cd3c185bd9e98bd829636931458f79";
    let msg = "06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76";
    let sign = "061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a";
    test_sign(Curve::Starkex, secret, msg, sign);
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
            tw_privkey.ptr(),
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
        tw_private_key_sign(
            tw_privkey.ptr(),
            std::ptr::null(),
            0,
            Curve::Secp256k1 as u32,
        )
        .into_vec()
    };
    assert!(actual.is_empty());
}

#[test]
fn test_tw_private_key_get_public_key_by_type() {
    #[track_caller]
    fn test_get_public_key_data_hex(tw_privkey: &TWPrivateKeyHelper, ty: PublicKeyType) -> String {
        let tw_pubkey =
            unsafe { tw_private_key_get_public_key_by_type(tw_privkey.ptr(), ty as u32) };
        assert!(!tw_pubkey.is_null());

        let actual = unsafe { tw_public_key_data(tw_pubkey).into_vec() };
        unsafe { tw_public_key_delete(tw_pubkey) };
        hex::encode(actual, false)
    }

    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
    );
    assert!(!tw_privkey.is_null());

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

    // Cardano private key.
    let privkey_bytes =
        hex::decode("089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e").unwrap();
    assert!(is_valid(privkey_bytes));

    // Zero private key.
    let privkey_bytes =
        H256::from("0000000000000000000000000000000000000000000000000000000000000000");
    assert!(!is_valid(privkey_bytes.into_vec()));
}
