// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;
use tw_encoding::hex;
use tw_encoding::hex::as_hex;
use tw_hash::{H256, H520};
use tw_keypair::ecdsa::secp256k1::{KeyPair, PrivateKey, PublicKey, VerifySignature};
use tw_keypair::traits::{SigningKeyTrait, VerifyingKeyTrait};

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
