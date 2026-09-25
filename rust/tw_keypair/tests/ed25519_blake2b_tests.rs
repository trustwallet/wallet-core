// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;
use tw_encoding::hex::{self, as_hex};
use tw_hash::{H256, H512};
use tw_keypair::ed25519::blake2b::KeyPair;
use tw_keypair::traits::{SigningKeyTrait, VerifyingKeyTrait};

/// The tests were generated in C++ using the `trezor-crypto` library.
const ED25519_BLAKE2B_SIGN: &str = include_str!("ed25519_blake2b_sign.json");

#[derive(Deserialize)]
struct Ed255191SignTest {
    #[serde(with = "as_hex")]
    secret: H256,
    msg: String,
    #[serde(with = "as_hex")]
    signature: H512,
}

#[test]
fn test_ed25519_blake2b_sign_verify() {
    let tests: Vec<Ed255191SignTest> = serde_json::from_str(ED25519_BLAKE2B_SIGN).unwrap();
    for test in tests {
        let msg = hex::decode(&test.msg).unwrap();

        let keypair = KeyPair::try_from(test.secret.as_slice()).unwrap();
        let actual = keypair.sign(msg.clone()).unwrap();
        assert_eq!(actual.to_bytes(), test.signature);

        assert!(keypair.verify(actual, msg));
    }
}
