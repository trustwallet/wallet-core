// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;
use tw_encoding::hex::{self, as_hex};
use tw_hash::{H256, H512};
use tw_keypair::ed25519::waves::KeyPair;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
use tw_misc::traits::ToBytesZeroizing;

/// The tests were generated in C++ using the `trezor-crypto` library.
const ED25519_WAVES_SIGN: &str = include_str!("ed25519_waves_sign.json");
const ED25519_WAVES_PRIV_TO_PUB: &str = include_str!("ed25519_waves_priv_to_pub.json");

#[derive(Deserialize)]
struct Ed255191WavesSignTest {
    #[serde(with = "as_hex")]
    secret: H256,
    msg: String,
    #[serde(with = "as_hex")]
    signature: H512,
}

#[derive(Deserialize)]
struct Ed255191WavesPrivToPubTest {
    #[serde(with = "as_hex")]
    secret: H256,
    #[serde(with = "as_hex")]
    public: H256,
}

#[test]
fn test_ed25519_waves_sign() {
    let tests: Vec<Ed255191WavesSignTest> = serde_json::from_str(ED25519_WAVES_SIGN).unwrap();
    for test in tests.into_iter() {
        let msg = hex::decode(&test.msg).unwrap();

        let keypair = KeyPair::try_from(test.secret.as_slice()).unwrap();
        let actual = keypair.sign(msg.clone()).unwrap();
        assert_eq!(actual.to_bytes(), test.signature);

        assert!(keypair.verify(actual, msg));
    }
}

#[test]
fn test_ed25519_waves_priv_to_pub() {
    let tests: Vec<Ed255191WavesPrivToPubTest> =
        serde_json::from_str(ED25519_WAVES_PRIV_TO_PUB).unwrap();

    for test in tests.into_iter() {
        let keypair = KeyPair::try_from(test.secret.as_slice()).unwrap();

        let private = keypair.private();
        assert_eq!(
            private.to_zeroizing_vec().as_slice(),
            test.secret.as_slice()
        );

        let public = keypair.public();
        assert_eq!(public.to_bytes(), test.public);
    }
}
