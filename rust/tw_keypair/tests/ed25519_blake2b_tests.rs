// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde::Deserialize;
use tw_encoding::hex;
use tw_hash::{H256, H512};
use tw_keypair::ed25519::blake2b::KeyPair;
use tw_keypair::traits::{SigningKeyTrait, VerifyingKeyTrait};

/// The tests were generated in C++ using the `trezor-crypto` library.
const ED25519_BLAKE2B_SIGN: &str = include_str!("ed25519_blake2b_sign.json");

#[derive(Deserialize)]
struct Ed255191SignTest {
    secret: H256,
    msg: String,
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
