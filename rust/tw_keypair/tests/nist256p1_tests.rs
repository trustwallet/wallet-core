// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde::Deserialize;
use tw_hash::{H256, H264, H520};
use tw_keypair::ecdsa::nist256p1::{PrivateKey, PublicKey, VerifySignature};
use tw_keypair::traits::VerifyingKeyTrait;

/// The tests were generated in C++ using the `trezor-crypto` library.
const NIST256P1_VERIFY: &str = include_str!("nist256p1_verify.json");
const NIST256P1_PRIV_TO_PUB_COMPRESSED: &str =
    include_str!("nist256p1_priv_to_pub_compressed.json");

#[derive(Deserialize)]
struct Nist256p1VerifyTest {
    public: H264,
    msg: H256,
    signature: H520,
}

#[derive(Deserialize)]
struct Nist256p1PrivToPubCompressedTest {
    secret: H256,
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
#[cfg(not(target_arch = "wasm32"))]
fn test_nist256p1_sign_verify_ring() {
    use ring::rand::{generate, SystemRandom};
    use ring::signature::{UnparsedPublicKey, ECDSA_P256_SHA256_FIXED};
    use tw_keypair::ecdsa::nist256p1::KeyPair;
    use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};

    let rng = SystemRandom::new();

    for _ in 0..1000 {
        let secret: [u8; 32] = generate(&rng).unwrap().expose();
        let msg_to_sign: [u8; 64] = generate(&rng).unwrap().expose();

        let hash_to_sign = tw_hash::sha2::sha256(&msg_to_sign);
        let hash_to_sign = H256::try_from(hash_to_sign.as_slice()).unwrap();

        let key_pair = KeyPair::try_from(secret.as_slice()).unwrap();
        let actual_sign = key_pair.sign(hash_to_sign).unwrap();

        let public_bytes = key_pair.public().uncompressed();

        let ring_public_key =
            UnparsedPublicKey::new(&ECDSA_P256_SHA256_FIXED, public_bytes.as_slice());

        ring_public_key
            .verify(&msg_to_sign, &actual_sign.to_bytes()[0..64])
            .unwrap();
    }
}
