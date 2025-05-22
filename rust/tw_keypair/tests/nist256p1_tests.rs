// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;
use tw_encoding::hex::{self, as_hex};
use tw_hash::{H256, H264, H520};
use tw_keypair::ecdsa::nist256p1::{PrivateKey, PublicKey, VerifySignature};
use tw_keypair::traits::VerifyingKeyTrait;
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
