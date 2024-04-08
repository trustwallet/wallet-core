// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;
use tw_encoding::hex::{self, as_hex};
use tw_hash::H512;
use tw_keypair::ed25519::cardano::ExtendedKeyPair;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};

/// The tests were generated in C++ using the `trezor-crypto` library.
const ED25519_EXTENDED_CARDANO_SIGN: &str = include_str!("ed25519_extended_cardano_sign.json");
const ED25519_EXTENDED_CARDANO_PRIV_TO_PUB: &str =
    include_str!("ed25519_extended_cardano_priv_to_pub.json");

#[derive(Deserialize)]
struct Ed255191ExtendedCardanoSignTest {
    secret: String,
    msg: String,
    #[serde(with = "as_hex")]
    signature: H512,
}

#[derive(Deserialize)]
struct Ed255191ExtendedCardanoPrivToPub {
    secret: String,
    public: String,
}

#[test]
fn test_ed25519_extended_cardano_sign() {
    let tests: Vec<Ed255191ExtendedCardanoSignTest> =
        serde_json::from_str(ED25519_EXTENDED_CARDANO_SIGN).unwrap();
    for test in tests.into_iter() {
        let msg = hex::decode(&test.msg).unwrap();
        let secret = hex::decode(&test.secret).unwrap();

        let keypair = ExtendedKeyPair::try_from(secret.as_slice()).unwrap();
        let actual = keypair.sign(msg.clone()).unwrap();
        assert_eq!(actual.to_bytes(), test.signature);

        assert!(keypair.verify(actual, msg));
    }
}

#[test]
fn test_ed25519_extended_cardano_priv_to_pub() {
    let tests: Vec<Ed255191ExtendedCardanoPrivToPub> =
        serde_json::from_str(ED25519_EXTENDED_CARDANO_PRIV_TO_PUB).unwrap();

    for test in tests.into_iter() {
        let secret = hex::decode(&test.secret).unwrap();

        let keypair = ExtendedKeyPair::try_from(secret.as_slice()).unwrap();
        assert_eq!(keypair.private().to_zeroizing_vec().as_slice(), secret);

        let public = keypair.public();
        assert_eq!(hex::encode(public.to_vec(), false), test.public);
    }
}
