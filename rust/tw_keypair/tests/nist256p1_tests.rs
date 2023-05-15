// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde::Deserialize;
use tw_hash::{H256, H264, H520};
use tw_keypair::ecdsa::nist256p1::{PublicKey, VerifySignature};
use tw_keypair::traits::VerifyingKeyTrait;

/// The tests were generated in C++ using the `trezor-crypto` library.
const NIST256P1_VERIFY: &str = include_str!("nist256p1_verify.json");

#[derive(Deserialize)]
struct Nist256p1VerifyTest {
    public: H264,
    msg: H256,
    signature: H520,
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
