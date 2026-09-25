// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use k256::Secp256k1;

mod keypair;
mod private;
mod public;

pub use keypair::KeyPair;
pub use private::PrivateKey;
pub use public::PublicKey;

pub type Signature = crate::ecdsa::signature::Signature<Secp256k1>;
pub type VerifySignature = crate::ecdsa::signature::VerifySignature<Secp256k1>;

#[cfg(test)]
mod tests {
    use super::*;
    use crate::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
    use tw_encoding::hex;
    use tw_encoding::hex::ToHex;
    use tw_hash::sha2::sha256;
    use tw_hash::sha3::keccak256;
    use tw_hash::{H256, H264, H520};
    use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};

    #[test]
    fn test_key_pair() {
        let secret =
            hex::decode("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5")
                .unwrap();
        let key_pair = KeyPair::try_from(secret.as_slice()).unwrap();
        assert_eq!(key_pair.private().to_zeroizing_vec().as_slice(), secret);
        assert_eq!(
            key_pair.public().compressed(),
            H264::from("0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1")
        );
    }

    #[test]
    fn test_key_pair_sign() {
        let key_pair =
            KeyPair::try_from("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5")
                .unwrap();

        let hash_to_sign = keccak256(b"hello");
        let hash_to_sign = H256::try_from(hash_to_sign.as_slice()).unwrap();
        let signature = key_pair.sign(hash_to_sign).unwrap();

        let expected = H520::from("8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901");
        assert_eq!(signature.to_bytes(), expected);

        let verify_signature = VerifySignature::from(signature);
        assert!(key_pair.verify(verify_signature, hash_to_sign));
    }

    #[test]
    fn test_private_key_from() {
        let hex = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
        let expected = hex::decode(hex).unwrap();

        // Test `From<&'static str>`.
        let private = PrivateKey::try_from(hex).unwrap();
        assert_eq!(private.to_zeroizing_vec().as_slice(), expected);

        // Test `From<&'a [u8]>`.
        let private = PrivateKey::try_from(expected.as_slice()).unwrap();
        assert_eq!(private.to_zeroizing_vec().as_slice(), expected);
    }

    #[test]
    fn test_private_key_sign_verify() {
        let secret = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
        let private = PrivateKey::try_from(secret).unwrap();
        let public = private.public();

        let hash_to_sign = keccak256(b"hello");
        let hash_to_sign = H256::try_from(hash_to_sign.as_slice()).unwrap();
        let signature = private.sign(hash_to_sign).unwrap();

        let expected = H520::from("8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901");
        assert_eq!(signature.to_bytes(), expected);

        let verify_signature = VerifySignature::from(signature);
        assert!(public.verify(verify_signature, hash_to_sign));
    }

    #[test]
    fn test_public_key_sign_verify_as_der() {
        let private_key = PrivateKey::try_from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();
        let public_key = private_key.public();

        let msg_to_sign = "Hello";
        let hash_to_sign = sha256(msg_to_sign.as_bytes());
        let hash_to_sign = H256::try_from(hash_to_sign.as_slice()).unwrap();

        let signature = private_key.sign(hash_to_sign).unwrap();
        let signature_der = VerifySignature::from(signature).to_der().unwrap();

        assert_eq!(signature_der.der_bytes().to_hex(), "304402200f5d5a9e5fc4b82a625312f3be5d3e8ad017d882de86c72c92fcefa924e894c102202071772a14201a3a0debf381b5e8dea39fadb9bcabdc02ee71ab018f55bf717f");

        // Now convert the der signature to ecdsa.
        let verify_signature = VerifySignature::from_der(signature_der).unwrap();
        assert!(public_key.verify(verify_signature, hash_to_sign));
    }

    #[test]
    fn test_public_key_from() {
        let compressed = "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1";
        let uncompressed = "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91";
        let expected_compressed = H264::from(compressed);
        let expected_uncompressed = H520::from(uncompressed);

        // From extended public key.
        let public = PublicKey::try_from(uncompressed).unwrap();
        assert_eq!(public.to_vec(), expected_compressed.into_vec());
        assert_eq!(public.compressed(), expected_compressed);
        assert_eq!(public.uncompressed(), expected_uncompressed);

        // From compressed public key.
        let public = PublicKey::try_from(compressed).unwrap();
        assert_eq!(public.to_vec(), expected_compressed.into_vec());
        assert_eq!(public.compressed(), expected_compressed);
        assert_eq!(public.uncompressed(), expected_uncompressed);
    }

    #[test]
    fn test_verify_invalid() {
        let secret = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
        let private = PrivateKey::try_from(secret).unwrap();

        let signature_bytes  = H520::from("375df53b6a4931dcf41e062b1c64288ed4ff3307f862d5c1b1c71964ce3b14c99422d0fdfeb2807e9900a26d491d5e8a874c24f98eec141ed694d7a433a90f0801");
        let verify_sig = VerifySignature::try_from(signature_bytes.as_slice()).unwrap();

        let hash_to_sign = keccak256(b"hello");
        let hash_to_sign = H256::try_from(hash_to_sign.as_slice()).unwrap();

        assert!(!private.public().verify(verify_sig, hash_to_sign));
    }

    #[test]
    fn test_shared_key_hash() {
        let private = PrivateKey::try_from(
            "9cd3b16e10bd574fed3743d8e0de0b7b4e6c69f3245ab5a168ef010d22bfefa0",
        )
        .unwrap();
        let public = PublicKey::try_from(
            "02a18a98316b5f52596e75bfa5ca9fa9912edd0c989b86b73d41bb64c9c6adb992",
        )
        .unwrap();
        let actual = private.shared_key_hash(&public);
        let expected =
            H256::from("ef2cf705af8714b35c0855030f358f2bee356ff3579cea2607b2025d80133c3a");
        assert_eq!(actual, expected);
    }

    #[test]
    fn test_public_key_recover() {
        let sign_bytes = H520::from("8720a46b5b3963790d94bcc61ad57ca02fd153584315bfa161ed3455e336ba624d68df010ed934b8792c5b6a57ba86c3da31d039f9612b44d1bf054132254de901");
        let sign = Signature::from_bytes(sign_bytes.as_slice()).unwrap();

        let signed_hash = keccak256(b"hello");
        let signed_hash = H256::try_from(signed_hash.as_slice()).unwrap();

        let actual = PublicKey::recover(sign, signed_hash).unwrap();
        let expected_compressed =
            H264::from("0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1");
        assert_eq!(actual.compressed(), expected_compressed);
    }
}
