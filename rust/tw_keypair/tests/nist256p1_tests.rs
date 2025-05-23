// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;
use tw_encoding::hex::{self, as_hex};
use tw_hash::{H256, H264, H520};
use tw_keypair::ecdsa::nist256p1::{PrivateKey, PublicKey, VerifySignature};
use tw_keypair::ffi::ecdsa::{tw_ecdsa_pubkey_hash, tw_ecdsa_sig_to_der};
use tw_keypair::traits::VerifyingKeyTrait;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_misc::traits::ToBytesVec;

/// The tests were generated in C++ using the `trezor-crypto` library.
const NIST256P1_VERIFY: &str = include_str!("nist256p1_verify.json");
const NIST256P1_PRIV_TO_PUB_COMPRESSED: &str =
    include_str!("nist256p1_priv_to_pub_compressed.json");

#[derive(Deserialize)]
struct Nist256p1VerifyTest {
    #[serde(with = "as_hex")]
    public: H264,
    #[serde(with = "as_hex")]
    msg: H256,
    #[serde(with = "as_hex")]
    signature: H520,
}

#[derive(Deserialize)]
struct Nist256p1PrivToPubCompressedTest {
    #[serde(with = "as_hex")]
    secret: H256,
    #[serde(with = "as_hex")]
    public: H264,
}

#[test]
fn test_nist256p1_verify() {
    let tests: Vec<Nist256p1VerifyTest> = serde_json::from_str(NIST256P1_VERIFY).unwrap();
    for test in tests {
        let public = PublicKey::try_from(test.public.as_slice()).unwrap();

        let verify_sign = VerifySignature::try_from(test.signature.as_slice()).unwrap();
        assert!(public.verify(verify_sign, test.msg));
    }
}

#[test]
fn test_nist256p1_priv_to_pub() {
    let tests: Vec<Nist256p1PrivToPubCompressedTest> =
        serde_json::from_str(NIST256P1_PRIV_TO_PUB_COMPRESSED).unwrap();
    for test in tests {
        let private = PrivateKey::try_from(test.secret.as_slice()).unwrap();
        let actual_public = private.public().compressed();

        assert_eq!(actual_public, test.public);
    }
}

#[test]
fn test_nist256p1_pubkey_compare() {
    let p1 = "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486";
    let p2 = "03d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1ee";
    let p3 = "0305947c8564734b0e634531c405a0b6488e2cb9bcde5eddefcf3f008f0c048115";

    let pubkey1 = PublicKey::try_from(hex::decode(p1).unwrap().as_slice()).unwrap();
    let pubkey2 = PublicKey::try_from(hex::decode(p2).unwrap().as_slice()).unwrap();
    let pubkey3 = PublicKey::try_from(hex::decode(p3).unwrap().as_slice()).unwrap();

    let mut pubkeys = vec![pubkey1, pubkey2, pubkey3];
    pubkeys.sort_by(|a, b| a.compare(b));
    assert_eq!(hex::encode(pubkeys[0].to_vec(), false), p3);
    assert_eq!(hex::encode(pubkeys[1].to_vec(), false), p1);
    assert_eq!(hex::encode(pubkeys[2].to_vec(), false), p2);
}

#[test]
fn test_nist256p1_shared_key() {
    let private_key = PrivateKey::try_from(
        hex::decode("bca2a4e7db34577e1193d6b6312244a246832228598c91fd5123cba52c182979")
            .unwrap()
            .as_slice(),
    )
    .unwrap();
    let public_key = PublicKey::try_from(
        hex::decode("031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486")
            .unwrap()
            .as_slice(),
    )
    .unwrap();
    let shared_key = private_key.ecies_shared_key(&public_key);
    assert_eq!(hex::encode(shared_key, false), "d5277ff8bda8bd043a663583019b9b4397b58c69a7fdbb9c39e6525eb99e5183f9a82cd4ce9f75d81ebc61ced5c763d612a9f8dc255ba4aea25675d882a8e514");
}

#[test]
fn test_tw_sig_to_der() {
    let signature = "6cc8a52ea475c5ab090bb91f62e1e3e9831450b6941d30ed0600a08acec014db65e7ecee3a3e1f95a53054a03f16f0e44f1d0aa3a44b87a4664819a0f5c0f38800";
    let signature_data = hex::decode(signature).unwrap();
    let tw_signature = TWDataHelper::create(signature_data);

    let signature = TWDataHelper::wrap(unsafe { tw_ecdsa_sig_to_der(tw_signature.ptr(), false) });

    assert_eq!(
        hex::encode(signature.to_vec().unwrap(), false),
        "304402206cc8a52ea475c5ab090bb91f62e1e3e9831450b6941d30ed0600a08acec014db022065e7ecee3a3e1f95a53054a03f16f0e44f1d0aa3a44b87a4664819a0f5c0f388"
    );
}

#[test]
fn test_tw_invalid_sig_to_der() {
    let signature = "6ccd";
    let signature_data = hex::decode(signature).unwrap();
    let tw_signature = TWDataHelper::create(signature_data);

    let signature = TWDataHelper::wrap(unsafe { tw_ecdsa_sig_to_der(tw_signature.ptr(), false) });
    assert_eq!(signature.is_null(), true);
}

#[test]
fn test_tw_pubkey_hash() {
    let pubkey = "031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486";
    let pubkey_data = hex::decode(pubkey).unwrap();
    let tw_pubkey = TWDataHelper::create(pubkey_data);

    let hash = TWDataHelper::wrap(unsafe { tw_ecdsa_pubkey_hash(tw_pubkey.ptr(), false, 1) });

    assert_eq!(
        hex::encode(hash.to_vec().unwrap(), false),
        "fc5f90ded54731474b8bb18ee579f77ad93427486a888d23a1af92719bcf5640"
    );
}
