// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;
use tw_encoding::hex;
use tw_encoding::hex::as_hex;
use tw_hash::{H256, H520};
use tw_keypair::ecdsa::secp256k1::{KeyPair, PrivateKey, PublicKey, VerifySignature};
use tw_keypair::ffi::ecdsa::{tw_ecdsa_pubkey_hash, tw_ecdsa_sig_to_der};
use tw_keypair::traits::{SigningKeyTrait, VerifyingKeyTrait};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_misc::traits::ToBytesVec;

/// The tests were generated in C++ using the `trezor-crypto` library.
const SECP256K1_SIGN: &str = include_str!("secp256k1_sign.json");

#[derive(Deserialize)]
struct Secp256k1SignTest {
    #[serde(with = "as_hex")]
    secret: H256,
    #[serde(with = "as_hex")]
    hash: H256,
    #[serde(with = "as_hex")]
    signature: H520,
}

#[test]
fn test_secp256k1_sign_verify() {
    let tests: Vec<Secp256k1SignTest> = serde_json::from_str(SECP256K1_SIGN).unwrap();
    for test in tests {
        let keypair = KeyPair::try_from(test.secret.as_slice()).unwrap();
        let actual = keypair.sign(test.hash).unwrap();
        assert_eq!(actual.to_bytes(), test.signature);

        let verify_sign = VerifySignature::from(actual);
        assert!(keypair.verify(verify_sign, test.hash));
    }
}

#[test]
fn test_secp256k1_pubkey_compare() {
    let p1 = "02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992";
    let p2 = "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1";

    let pubkey1 = PublicKey::try_from(hex::decode(p1).unwrap().as_slice()).unwrap();
    let pubkey2 = PublicKey::try_from(hex::decode(p2).unwrap().as_slice()).unwrap();

    let mut pubkeys = vec![pubkey1, pubkey2];
    pubkeys.sort_by(|a, b| a.compare(b));
    assert_eq!(hex::encode(pubkeys[0].to_vec(), false), p2);
    assert_eq!(hex::encode(pubkeys[1].to_vec(), false), p1);
}

#[test]
fn test_secp256k1_shared_key() {
    let private_key = PrivateKey::try_from(
        hex::decode("2bd806c97f0e00af1a1fc3328fa763a9269723c8db8fac4f93af71db186d6e90")
            .unwrap()
            .as_slice(),
    )
    .unwrap();
    let public_key = PublicKey::try_from(
        hex::decode("024edfcf9dfe6c0b5c83d1ab3f78d1b39a46ebac6798e08e19761f5ed89ec83c10")
            .unwrap()
            .as_slice(),
    )
    .unwrap();
    let shared_key = private_key.ecies_shared_key(&public_key);
    assert_eq!(hex::encode(shared_key, false), "a71b4ec5a9577926a1d2aa1d9d99327fd3b68f6a1ea597200a0d890bd3331df300a2d49fec0b2b3e6969ce9263c5d6cf47c191c1ef149373ecc9f0d98116b598");
}

#[test]
fn test_tw_sig_to_der() {
    let signature = "882d92979f3fd4df2b19eecea6b4ca3104898774e83506d934736a80697a19366f4fde413d1fcf73adc00dd3938be427cea62a2aa166eab209703a3f63bd77fc00";
    let signature_data = hex::decode(signature).unwrap();
    let tw_signature = TWDataHelper::create(signature_data);

    let signature = TWDataHelper::wrap(unsafe { tw_ecdsa_sig_to_der(tw_signature.ptr(), true) });

    assert_eq!(
        hex::encode(signature.to_vec().unwrap(), false),
        "3045022100882d92979f3fd4df2b19eecea6b4ca3104898774e83506d934736a80697a193602206f4fde413d1fcf73adc00dd3938be427cea62a2aa166eab209703a3f63bd77fc"
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

    let hash = TWDataHelper::wrap(unsafe { tw_ecdsa_pubkey_hash(tw_pubkey.ptr(), true, 1) });

    assert_eq!(
        hex::encode(hash.to_vec().unwrap(), false),
        "fc5f90ded54731474b8bb18ee579f77ad93427486a888d23a1af92719bcf5640"
    );
}
