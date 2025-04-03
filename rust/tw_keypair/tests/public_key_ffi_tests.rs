// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex;
use tw_hash::sha2::sha256;
use tw_hash::sha3::keccak256;
use tw_keypair::ffi::privkey::{
    tw_private_key_get_public_key_by_type, tw_private_key_sign, tw_private_key_sign_as_der
};
use tw_keypair::ffi::pubkey::{
    tw_public_key_compressed, tw_public_key_data, tw_public_key_delete, tw_public_key_extended,
    tw_public_key_recover_from_signature, tw_public_key_type, tw_public_key_verify,
    tw_public_key_verify_as_der,
};
use tw_keypair::test_utils::tw_private_key_helper::TWPrivateKeyHelper;
use tw_keypair::test_utils::tw_public_key_helper::TWPublicKeyHelper;
use tw_keypair::tw::{Curve, PublicKeyType};
use tw_memory::ffi::c_byte_array::CByteArray;

fn test_verify(ty: PublicKeyType, public: &str, msg: &str, sign: &str) {
    let tw_public = TWPublicKeyHelper::with_hex(public, ty);
    assert!(!tw_public.is_null());

    let signature_bytes = hex::decode(sign).unwrap();
    let signature_raw = CByteArray::from(signature_bytes);
    let msg = hex::decode(msg).unwrap();
    let msg_raw = CByteArray::from(msg);

    let valid = unsafe {
        tw_public_key_verify(
            tw_public.ptr(),
            signature_raw.data(),
            signature_raw.size(),
            msg_raw.data(),
            msg_raw.size(),
        )
    };
    assert!(valid);
}

#[test]
fn test_tw_public_key_create_by_type() {
    let tw_public = TWPublicKeyHelper::with_hex(
        "02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992",
        PublicKeyType::Secp256k1,
    );
    assert!(!tw_public.is_null());
    let ty = unsafe { tw_public_key_type(tw_public.ptr()) };
    assert_eq!(ty, PublicKeyType::Secp256k1 as u32);

    // Compressed pubkey with '03' prefix.
    let tw_public = TWPublicKeyHelper::with_hex(
        "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1",
        PublicKeyType::Secp256k1,
    );
    assert!(!tw_public.is_null());
    let ty = unsafe { tw_public_key_type(tw_public.ptr()) };
    assert_eq!(ty, PublicKeyType::Secp256k1 as u32);

    let tw_public = TWPublicKeyHelper::with_hex(
        "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91",
        PublicKeyType::Secp256k1Extended,
    );
    assert!(!tw_public.is_null());
    let ty = unsafe { tw_public_key_type(tw_public.ptr()) };
    assert_eq!(ty, PublicKeyType::Secp256k1Extended as u32);

    // Pass an extended pubkey, but Secp256k1 type.
    let tw_public = TWPublicKeyHelper::with_hex(
        "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91",
        PublicKeyType::Secp256k1,
    );
    assert!(tw_public.is_null());
    let ty = unsafe { tw_public_key_type(tw_public.ptr()) };
    assert_eq!(ty, PublicKeyType::Secp256k1 as u32);

    // Pass a compressed pubkey, but Secp256k1Extended type.
    let tw_public = TWPublicKeyHelper::with_hex(
        "02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992",
        PublicKeyType::Secp256k1Extended,
    );
    assert!(tw_public.is_null());
    let ty = unsafe { tw_public_key_type(tw_public.ptr()) };
    assert_eq!(ty, PublicKeyType::Secp256k1 as u32);
}

#[test]
fn test_tw_public_key_delete_null() {
    unsafe { tw_public_key_delete(std::ptr::null_mut()) };
}

#[test]
fn test_tw_public_key_verify_secp256k1() {
    let public = "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1";
    let msg = hex::encode(keccak256(b"hello").as_slice(), false);
    let sign = "8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901";
    test_verify(PublicKeyType::Secp256k1, public, &msg, sign);
}

#[test]
fn test_tw_public_key_verify_nist256p1() {
    let public = "026d786ab8fda678cf50f71d13641049a393b325063b8c0d4e5070de48a2caf9ab";
    let msg = hex::encode(keccak256(b"hello").as_slice(), false);
    let sign = "8859e63a0c0cc2fc7f788d7e78406157b288faa6f76f76d37c4cd1534e8d83c468f9fd6ca7dde378df594625dcde98559389569e039282275e3d87c26e36447401";
    test_verify(PublicKeyType::Nist256p1, public, &msg, sign);
}

#[test]
fn test_tw_public_key_verify_ed25519() {
    let public = "4870d56d074c50e891506d78faa4fb69ca039cc5f131eb491e166b975880e867";
    let msg = hex::encode(sha256(b"Hello").as_slice(), false);
    let sign = "42848abf2641a731e18b8a1fb80eff341a5acebdc56faeccdcbadb960aef775192842fccec344679446daa4d02d264259c8f9aa364164ebe0ebea218581e2e03";
    test_verify(PublicKeyType::Ed25519, public, &msg, sign);
}

#[test]
fn test_tw_public_key_verify_ed25519_blake2b() {
    let public = "b689ab808542e13f3d2ec56fe1efe43a1660dcadc73ce489fde7df98dd8ce5d9";
    let msg = hex::encode(sha256(b"Hello").as_slice(), false);
    let sign = "5c1473944cd0234ebc5a91b2966b9e707a33b936dadd149417a2e53b6b3fc97bef17b767b1690708c74d7b4c8fe48703fd44a6ef59d4cc5b9f88ba992db0a003";
    test_verify(PublicKeyType::Ed25519Blake2b, public, &msg, sign);
}

#[test]
fn test_tw_public_key_verify_curve25519_waves() {
    let public = "559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d";
    let msg = "0402559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d00000000016372e852120000000005f5e1000000000005f5e10001570acc4110b78a6d38b34d879b5bba38806202ecf1732f8542000766616c6166656c";
    let sign = "af7989256f496e103ce95096b3f52196dd9132e044905fe486da3b829b5e403bcba95ab7e650a4a33948c2d05cfca2dce4d4df747e26402974490fb4c49fbe8f";
    test_verify(PublicKeyType::Curve25519Waves, public, msg, sign);
}

#[test]
fn test_tw_public_key_verify_ed25519_extended_cardano() {
    let public = "57fd54be7b38bb8952782c2f59aa276928a4dcbb66c8c62ce44f9d623ecd5a03bf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4\
    06465638ee0e1ca1a9f34d940a0c8c48bbaaa0db124107e4c1b12b872a67511fed7f28be986cbe06819165f2ee41b403678a098961013cf4a2f3e9ea61fb6c1a";
    let msg = hex::encode(keccak256(b"hello").as_slice(), false);
    let sign = "375df53b6a4931dcf41e062b1c64288ed4ff3307f862d5c1b1c71964ce3b14c99422d0fdfeb2807e9900a26d491d5e8a874c24f98eec141ed694d7a433a90f08";
    test_verify(PublicKeyType::Ed25519ExtendedCardano, public, &msg, sign);
}

#[test]
fn test_tw_public_key_compressed_extended() {
    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        Curve::Secp256k1.to_raw(),
    );
    assert!(!tw_privkey.is_null());

    let tw_pubkey = unsafe {
        TWPublicKeyHelper::wrap(tw_private_key_get_public_key_by_type(
            tw_privkey.ptr(),
            PublicKeyType::Secp256k1 as u32,
        ))
    };
    assert!(!tw_pubkey.is_null());

    unsafe {
        assert_eq!(
            tw_public_key_type(tw_pubkey.ptr()),
            PublicKeyType::Secp256k1 as u32
        );
    }

    let tw_extended = unsafe { TWPublicKeyHelper::wrap(tw_public_key_extended(tw_pubkey.ptr())) };
    assert!(!tw_extended.is_null());

    unsafe {
        assert_eq!(
            tw_public_key_type(tw_extended.ptr()),
            PublicKeyType::Secp256k1Extended as u32
        );
    }

    let tw_compressed =
        unsafe { TWPublicKeyHelper::wrap(tw_public_key_compressed(tw_extended.ptr())) };
    assert!(!tw_compressed.is_null());

    unsafe {
        assert_eq!(
            tw_public_key_type(tw_compressed.ptr()),
            PublicKeyType::Secp256k1 as u32
        );
    }
}

#[test]
fn test_tw_public_key_recover() {
    let message =
        hex::decode("de4e9524586d6fce45667f9ff12f661e79870c4105fa0fb58af976619bb11432").unwrap();
    let signature = hex::decode("00000000000000000000000000000000000000000000000000000000000000020123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef80").unwrap();

    let message_raw = CByteArray::from(message);
    let signature_raw = CByteArray::from(signature);

    let tw_public_key = unsafe {
        TWPublicKeyHelper::wrap(tw_public_key_recover_from_signature(
            signature_raw.data(),
            signature_raw.size(),
            message_raw.data(),
            message_raw.size(),
            0x01,
        ))
    };

    assert!(!tw_public_key.is_null());

    unsafe {
        assert_eq!(
            tw_public_key_type(tw_public_key.ptr()),
            PublicKeyType::Secp256k1Extended as u32
        );

        let public_key_data = tw_public_key_data(tw_public_key.ptr());
        let public_key_hex = hex::encode(public_key_data.into_vec(), false);

        assert_eq!(
            public_key_hex,
            "0456d8089137b1fd0d890f8c7d4a04d0fd4520a30b19518ee87bd168ea12ed8090329274c4c6c0d9df04515776f2741eeffc30235d596065d718c3973e19711ad0"
        );
    }
}

#[test]
fn test_tw_public_key_verify_as_der() {
    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        Curve::Secp256k1.to_raw(),
    );
    assert!(!tw_privkey.is_null());

    let message = "Hello";
    let message_data = message.as_bytes();
    let digest = keccak256(message_data);
    let digest_raw = CByteArray::from(digest.to_vec());

    // Sign the digest using DER format
    let signature = unsafe {
        tw_private_key_sign_as_der(tw_privkey.ptr(), digest_raw.data(), digest_raw.size())
            .into_vec()
    };
    let signature_raw = CByteArray::from(signature);

    // Get the public key
    let tw_public_key = unsafe {
        TWPublicKeyHelper::wrap(tw_private_key_get_public_key_by_type(
            tw_privkey.ptr(),
            PublicKeyType::Secp256k1Extended as u32,
        ))
    };

    // Verify the DER signature
    let is_valid_der = unsafe {
        tw_public_key_verify_as_der(
            tw_public_key.ptr(),
            signature_raw.data(),
            signature_raw.size(),
            digest_raw.data(),
            digest_raw.size(),
        )
    };
    assert!(is_valid_der, "DER signature verification failed");

    // Regular verification should fail with DER signature
    let is_valid_regular = unsafe {
        tw_public_key_verify(
            tw_public_key.ptr(),
            signature_raw.data(),
            signature_raw.size(),
            digest_raw.data(),
            digest_raw.size(),
        )
    };
    assert!(
        !is_valid_regular,
        "Regular verification should fail with DER signature"
    );
}

#[test]
fn test_tw_public_key_verify_zilliqa() {
    let tw_privkey = TWPrivateKeyHelper::with_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        Curve::ZilliqaSchnorr.to_raw(),
    );
    assert!(!tw_privkey.is_null());

    let message = "hello schnorr";
    let message_data = message.as_bytes();
    let digest = sha256(message_data);
    let digest_raw = CByteArray::from(digest.to_vec());

    // Sign the digest using Zilliqa format
    let signature = unsafe {
        tw_private_key_sign(tw_privkey.ptr(), digest_raw.data(), digest_raw.size())
            .into_vec()
    };
    let signature_raw = CByteArray::from(signature.clone());

    // Get the public key
    let tw_public_key = unsafe {
        TWPublicKeyHelper::wrap(tw_private_key_get_public_key_by_type(
            tw_privkey.ptr(),
            PublicKeyType::ZilliqaSchnorr as u32,
        ))
    };

    // Verify the Zilliqa signature
    let is_valid = unsafe {
        tw_public_key_verify(
            tw_public_key.ptr(),
            signature_raw.data(),
            signature_raw.size(),
            digest_raw.data(),
            digest_raw.size(),
        )
    };
    assert!(is_valid, "Zilliqa signature verification failed");

    // Check the expected signature
    assert_eq!(
        hex::encode(signature, false),
        "b8118ccb99563fe014279c957b0a9d563c1666e00367e9896fe541765246964f64a53052513da4e6dc20fdaf69ef0d95b4ca51c87ad3478986cf053c2dd0b853"
    );
}
