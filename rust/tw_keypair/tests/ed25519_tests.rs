// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;
use tw_encoding::hex;
use tw_hash::{H256, H512};
use tw_keypair::ed25519::sha512::KeyPair;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
use tw_misc::traits::ToBytesZeroizing;

/// The tests were generated in C++ using the `trezor-crypto` library.
const ED25519_SIGN: &str = include_str!("ed25519_sign.json");
const ED25519_PRIV_TO_PUB: &str = include_str!("ed25519_priv_to_pub.json");

#[derive(Deserialize)]
struct Ed255191SignTest {
    secret: H256,
    msg: String,
    signature: H512,
}

#[derive(Deserialize)]
struct Ed255191PrivToPubTest {
    secret: H256,
    public: H256,
}

#[test]
fn test_ed25519_sign_verify() {
    let tests: Vec<Ed255191SignTest> = serde_json::from_str(ED25519_SIGN).unwrap();
    for test in tests {
        let msg = hex::decode(&test.msg).unwrap();

        let keypair = KeyPair::try_from(test.secret.as_slice()).unwrap();
        let actual = keypair.sign(msg.clone()).unwrap();
        assert_eq!(actual.to_bytes(), test.signature);

        assert!(keypair.verify(actual, msg));
    }
}

#[test]
fn test_ed25519_priv_to_pub() {
    let tests: Vec<Ed255191PrivToPubTest> = serde_json::from_str(ED25519_PRIV_TO_PUB).unwrap();
    for test in tests {
        let keypair = KeyPair::try_from(test.secret.as_slice()).unwrap();
        assert_eq!(
            keypair.private().to_zeroizing_vec().as_slice(),
            test.secret.as_slice()
        );

        let public = keypair.public();
        assert_eq!(public.to_bytes(), test.public);
    }
}
