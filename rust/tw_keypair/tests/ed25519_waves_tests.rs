// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;
use tw_encoding::hex::{self, as_hex};
use tw_hash::{H256, H512};
use tw_keypair::ed25519::waves::KeyPair;
use tw_keypair::ffi::curve25519::tw_curve25519_pubkey_to_ed25519;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
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

#[test]
fn test_tw_curve25519_pubkey_to_ed25519() {
    let pubkey = "559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d";
    let pubkey_data = hex::decode(pubkey).unwrap();
    let tw_pubkey = TWDataHelper::create(pubkey_data);

    let hash = TWDataHelper::wrap(unsafe { tw_curve25519_pubkey_to_ed25519(tw_pubkey.ptr()) });

    assert_eq!(
        hex::encode(hash.to_vec().unwrap(), false),
        "ff84c4bfc095df25b01e48807715856d95af93d88c5b57f30cb0ce567ca4ce56"
    );
}
