// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use digest::{consts::U64, Digest};

mod private;
mod public;
mod secret;
mod signature;

pub use signature::Signature;

pub mod sha512 {
    use sha2::Sha512;

    pub type PrivateKey = crate::ed25519::private::PrivateKey<Sha512>;
    pub type PublicKey = crate::ed25519::public::PublicKey<Sha512>;
}

pub mod blake2b {
    use blake2::Blake2b;

    pub type PrivateKey = crate::ed25519::private::PrivateKey<Blake2b>;
    pub type PublicKey = crate::ed25519::public::PublicKey<Blake2b>;
}

pub trait Hash512: Digest<OutputSize = U64> {
    const OUTPUT_LEN: usize = 64;
    const HALF_LEN: usize = 32;
}

impl<T> Hash512 for T where T: Digest<OutputSize = U64> {}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::traits::{SigningKeyTrait, VerifyingKeyTrait};
    use tw_encoding::hex;
    use tw_hash::sha2::sha256;
    use tw_hash::{H256, H512};

    #[test]
    fn test_private_to_public() {
        let private = sha512::PrivateKey::from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        );
        let public = private.public();

        let expected =
            H256::from("4870d56d074c50e891506d78faa4fb69ca039cc5f131eb491e166b975880e867");
        assert_eq!(public.to_bytes(), expected);
    }

    #[test]
    fn test_private_to_public_blake2b() {
        let private = blake2b::PrivateKey::from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        );
        let public = private.public();

        let expected =
            H256::from("b689ab808542e13f3d2ec56fe1efe43a1660dcadc73ce489fde7df98dd8ce5d9");
        assert_eq!(public.to_bytes(), expected);
    }

    #[test]
    fn test_keypair_sign_verify() {
        let keypair = sha512::PrivateKey::from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        );
        let to_sign = sha256(b"Hello");
        let actual = keypair.sign(to_sign.clone()).unwrap();

        let expected = H512::from("42848abf2641a731e18b8a1fb80eff341a5acebdc56faeccdcbadb960aef775192842fccec344679446daa4d02d264259c8f9aa364164ebe0ebea218581e2e03");
        assert_eq!(actual.to_bytes(), expected);

        assert!(keypair.public().verify(actual, to_sign));
    }

    #[test]
    fn test_signature_malleability() {
        let pubkey = sha512::PublicKey::from(
            "a96e02312b03116ff88a9f3e7cea40f424af43a5c6ca6c8ed4f98969faf46ade",
        );
        let message = b"Hello, world!".to_vec();

        let orig_sign_bytes = hex::decode("ea85a47dcc18b512dfea7c209162abaea4808d77c1ec903dc7ba6e2afa3f9f07e7ed7a20a4e2fa1009db3d1443e937e6abb16ff3c3eaecb798faed7fbb40b008").unwrap();
        Signature::try_from(orig_sign_bytes.as_slice()).unwrap();

        let modified_sign_bytes = hex::decode("ea85a47dcc18b512dfea7c209162abaea4808d77c1ec903dc7ba6e2afa3f9f07d4c1707dbe450d69df7735b721e316fbabb16ff3c3eaecb798faed7fbb40b018").unwrap();
        Signature::try_from(modified_sign_bytes.as_slice()).unwrap_err();
    }

    #[test]
    fn test_keypair_sign_verify_blake2b() {
        let keypair = blake2b::PrivateKey::from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        );
        let to_sign = sha256(b"Hello");
        let actual = keypair.sign(to_sign.clone()).unwrap();

        let expected = H512::from("5c1473944cd0234ebc5a91b2966b9e707a33b936dadd149417a2e53b6b3fc97bef17b767b1690708c74d7b4c8fe48703fd44a6ef59d4cc5b9f88ba992db0a003");
        assert_eq!(actual.to_bytes(), expected);

        assert!(keypair.public().verify(actual, to_sign));
    }
}

#[cfg(test)]
mod tests_original {
    use curve25519_dalek::constants;
    use curve25519_dalek::edwards::CompressedEdwardsY;
    use curve25519_dalek::scalar::Scalar;
    use ed25519_dalek::ed25519::signature::SignerMut;
    use ed25519_dalek::{ExpandedSecretKey, Keypair, PublicKey, SecretKey, Sha512, Verifier};
    use tw_hash::sha2::sha256;
    use tw_hash::{H256, H512};
    // use bla
    use blake2::Digest;
    use tw_hash::sha3::keccak256;

    #[test]
    fn test_verify() {
        let public =
            tw_encoding::base64::decode("qW4CMSsDEW/4ip8+fOpA9CSvQ6XGymyO1PmJafr0at4=", false)
                .unwrap();
        println!("{}", tw_encoding::hex::encode(&public, false));
        let public = PublicKey::from_bytes(&public).unwrap();
        let sign_1 = tw_encoding::base64::decode("6oWkfcwYtRLf6nwgkWKrrqSAjXfB7JA9x7puKvo/nwfn7XogpOL6EAnbPRRD6Tfmq7Fv88Pq7LeY+u1/u0CwCA==", false).unwrap();
        println!("{}", tw_encoding::hex::encode(&sign_1, false));
        let sign_2 = tw_encoding::base64::decode("6oWkfcwYtRLf6nwgkWKrrqSAjXfB7JA9x7puKvo/nwfUwXB9vkUNad93Nbch4xb7q7Fv88Pq7LeY+u1/u0CwGA==", false).unwrap();
        println!("{}", tw_encoding::hex::encode(&sign_2, false));

        let sign_1 = ed25519_dalek::Signature::from_bytes(&sign_1).unwrap();
        let sign_2 = ed25519_dalek::Signature::from_bytes(&sign_2).unwrap();

        public.verify(b"Hello, world!", &sign_1).unwrap();
        public.verify_strict(b"Hello, world!", &sign_2).unwrap_err();
    }

    // msg: [24, 95, 141, 179, 34, 113, 254, 37, 245, 97, 166, 252, 147, 139, 46, 38, 67, 6, 236, 48, 78, 218, 81, 128, 7, 209, 118, 72, 38, 56, 25, 105, ]
    // secret (key): [40, 243, 55, 77, 234, 67, 126, 254, 10, 189, 141, 149, 157, 66, 54, 128, 201, 75, 137, 164, 106, 182, 78, 20, 130, 0, 221, 79, 144, 220, 197, 93, ]
    // secret ext (nonce): [83, 175, 135, 213, 135, 207, 45, 201, 192, 26, 248, 107, 123, 41, 109, 158, 209, 49, 62, 50, 146, 72, 54, 184, 52, 170, 65, 83, 177, 247, 110, 231, ]
    // rA: [114, 205, 167, 10, 17, 109, 116, 36, 143, 44, 72, 56, 71, 124, 48, 57, 187, 135, 255, 0, 203, 117, 6, 31, 2, 26, 217, 22, 181, 0, 14, 53, 222, 10, 0, 0, 76, 0, 244, 117, 169, 99, 42, 224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 41, 118, 111, ]
    // S: [146, 132, 47, 12, 179, 211, 24, 37, 71, 212, 166, 26, 147, 128, 52, 25, 37, 156, 143, 26, 142, 146, 89, 56, 228, 235, 224, 43, 40, 6, 150, 7, 46, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ]
    //
    // Public: [72, 112, 213, 109, 7, 76, 80, 232, 145, 80, 109, 120, 250, 164, 251, 105, 202, 3, 156, 197, 241, 49, 235, 73, 30, 22, 107, 151, 88, 128, 232, 103]
    #[test]
    fn test_ed25519_sign_verify_as_ext() {
        let secret = H256::from("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
        let private = SecretKey::from_bytes(secret.as_slice()).unwrap();
        let public = PublicKey::from(&private);
        // let hash = Sha256::default().chain_update(b"Hello");
        let hash = sha256(b"Hello");

        let expanded_secret = ExpandedSecretKey::from(&private);
        let secret_bytes = expanded_secret.to_bytes();
        let key = H256::try_from(&secret_bytes[..32]).unwrap();
        let nonce = H256::try_from(&secret_bytes[32..]).unwrap();

        // let mut keypair = Keypair::from_bytes(secret.as_slice()).unwrap();
        let actual = sign_ed25519_ext(key, nonce, &public, &hash);
        // let actual = keypair.sign_prehashed(hash, None).unwrap();
        let expected = H512::from("42848abf2641a731e18b8a1fb80eff341a5acebdc56faeccdcbadb960aef775192842fccec344679446daa4d02d264259c8f9aa364164ebe0ebea218581e2e03");
        assert_eq!(actual, expected);

        // assert!(keypair.verify(&hash, &actual).is_ok());
    }

    #[test]
    fn test_ed25519_sign_verify() {
        let secret = H256::from("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
        let private = SecretKey::from_bytes(secret.as_slice()).unwrap();
        let public = PublicKey::from(&private);
        // let hash = Sha256::default().chain_update(b"Hello");
        let hash = sha256(b"Hello");

        let mut keypair = Keypair {
            secret: private,
            public,
        };

        // let mut keypair = Keypair::from_bytes(secret.as_slice()).unwrap();
        let actual = keypair.sign(&hash);
        // let actual = keypair.sign_prehashed(hash, None).unwrap();
        let expected = H512::from("42848abf2641a731e18b8a1fb80eff341a5acebdc56faeccdcbadb960aef775192842fccec344679446daa4d02d264259c8f9aa364164ebe0ebea218581e2e03");
        assert_eq!(actual.to_bytes(), expected.take());

        assert!(keypair.verify(&hash, &actual).is_ok());
    }

    fn sign_ed25519_ext(key: H256, nonce: H256, public: &PublicKey, message: &[u8]) -> H512 {
        let key = Scalar::from_bits(key.take());

        let mut h: Sha512 = Sha512::new();
        let R: CompressedEdwardsY;
        let r: Scalar;
        let s: Scalar;
        let k: Scalar;

        h.update(&nonce);
        h.update(&message);

        // rA
        r = Scalar::from_hash(h);
        R = (&r * &constants::ED25519_BASEPOINT_TABLE).compress();

        h = Sha512::new();
        h.update(R.as_bytes());
        h.update(&public.to_bytes());
        h.update(&message);

        k = Scalar::from_hash(h);
        s = &(&k * key) + &r;

        let mut signature_bytes: H512 = H512::new();

        signature_bytes[..32].copy_from_slice(&R.as_bytes()[..]);
        signature_bytes[32..].copy_from_slice(&s.as_bytes()[..]);
        signature_bytes
    }

    #[test]
    fn test_ed25519_cardano_sign_verify() {
        let secret = H256::from("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744");
        let nonce = H256::from("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff");

        let expanded_bytes = H512::from("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff");
        let expanded = ExpandedSecretKey::from_bytes(expanded_bytes.as_slice()).unwrap();
        let public = PublicKey::from(&expanded);

        let hash = keccak256(b"hello");

        let actual = sign_ed25519_ext(secret, nonce, &public, &hash);
        let expected = H512::from("375df53b6a4931dcf41e062b1c64288ed4ff3307f862d5c1b1c71964ce3b14c99422d0fdfeb2807e9900a26d491d5e8a874c24f98eec141ed694d7a433a90f08");
        assert_eq!(actual, expected);

        let signature = ed25519_dalek::Signature::from(actual.take());
        public.verify(&hash, &signature).unwrap();
    }

    // #[test]
    // fn test_ed25519_cardano_sign_verify() {
    //     let secret = H256::from("b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744");
    //     let public_bytes =
    //         H256::from("309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71eff");
    //
    //     let private = SecretKey::from_bytes(secret.as_slice()).unwrap();
    //     let public = PublicKey::from_bytes(public_bytes.as_slice()).unwrap();
    //     // let hash = Sha256::default().chain_update(b"Hello");
    //     let hash = sha256(b"Hello");
    //
    //     let expanded_priv = ExpandedSecretKey::from(&private);
    //
    //     // let mut keypair = Keypair::from_bytes(secret.as_slice()).unwrap();
    //     let actual = expanded_priv.sign(&hash, &public);
    //     // let actual = keypair.sign_prehashed(hash, None).unwrap();
    //     let expected = H512::from("375df53b6a4931dcf41e062b1c64288ed4ff3307f862d5c1b1c71964ce3b14c99422d0fdfeb2807e9900a26d491d5e8a874c24f98eec141ed694d7a433a90f08");
    //     assert_eq!(actual.to_bytes(), expected.take());
    //
    //     // assert!(keypair.verify(&hash, &actual).is_ok());
    // }

    // fn sign_ed25519_blake2b_nano(keypair: &Keypair, message: &[u8]) -> H512 {
    //     let private = ExpandedSecretKey::from(&keypair.secret);
    //     let secret_bytes = private.to_bytes();
    //     let mut key_bytes = [0; 32];
    //     key_bytes.copy_from_slice(&secret_bytes[..32]);
    //     let key = Scalar::from_bits(key_bytes);
    //     let nonce = &secret_bytes[32..];
    //
    //     let mut h: Blake2b = Blake2b::new();
    //     let R: CompressedEdwardsY;
    //     let r: Scalar;
    //     let s: Scalar;
    //     let k: Scalar;
    //
    //     h.update(nonce);
    //     h.update(&message);
    //
    //     r = Scalar::from_hash(h);
    //     R = (&r * &constants::ED25519_BASEPOINT_TABLE).compress();
    //
    //     h = Blake2b::new();
    //     h.update(R.as_bytes());
    //     h.update(keypair.public.as_bytes());
    //     h.update(&message);
    //
    //     k = Scalar::from_hash(h);
    //     s = &(&k * key) + &r;
    //
    //     let mut signature_bytes: H512 = H512::new();
    //
    //     signature_bytes[..32].copy_from_slice(&R.as_bytes()[..]);
    //     signature_bytes[32..].copy_from_slice(&s.as_bytes()[..]);
    //     signature_bytes
    // }
    //
    // // s: 151, 132, 41, 127, 161, .., 61, 225, 15
    // // nonce: 83, 175, 135, 213, 135
    // #[test]
    // fn test_ed25519_blake_sign_verify() {
    //     let secret = H256::from("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
    //     let private = SecretKey::from_bytes(secret.as_slice()).unwrap();
    //     let public = PublicKey::from(&private);
    //     // let hash = Sha256::default().chain_update(b"Hello");
    //     let hash = sha256(b"Hello");
    //
    //     let mut keypair = Keypair {
    //         secret: private,
    //         public,
    //     };
    //
    //     // let mut keypair = Keypair::from_bytes(secret.as_slice()).unwrap();
    //     let actual = sign_ed25519_blake2b_nano(&keypair, &hash);
    //     // let actual = keypair.sign_prehashed(hash, None).unwrap();
    //     let expected = H512::from("5c1473944cd0234ebc5a91b2966b9e707a33b936dadd149417a2e53b6b3fc97bef17b767b1690708c74d7b4c8fe48703fd44a6ef59d4cc5b9f88ba992db0a003");
    //     assert_eq!(actual, expected);
    //
    //     // assert!(keypair.verify(&hash, &actual).is_ok());
    // }
}

#[cfg(test)]
mod tests_blake {
    use ed25519_dalek_blake2b::{Keypair, PublicKey, SecretKey, Signer};
    use tw_hash::sha2::sha256;
    use tw_hash::{H256, H512};

    // r: 104, 159, 152, 208, 85
    // R: 92, 20, 115, 148, 76, 208
    // nonce: 53, 233, 42, 234, 86, 117, 73
    #[test]
    fn test_ed25519_blake_sign_verify() {
        let secret = H256::from("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
        let private = SecretKey::from_bytes(secret.as_slice()).unwrap();
        let public = PublicKey::from(&private);
        // let hash = Sha256::default().chain_update(b"Hello");
        let hash = sha256(b"Hello");

        let keypair = Keypair {
            secret: private,
            public,
        };

        // let mut keypair = Keypair::from_bytes(secret.as_slice()).unwrap();
        let actual = keypair.sign(&hash);
        // let actual = keypair.sign_prehashed(hash, None).unwrap();
        let expected = H512::from("5c1473944cd0234ebc5a91b2966b9e707a33b936dadd149417a2e53b6b3fc97bef17b767b1690708c74d7b4c8fe48703fd44a6ef59d4cc5b9f88ba992db0a003");
        assert_eq!(actual.to_bytes(), expected.take());

        assert!(keypair.verify(&hash, &actual).is_ok());
    }
}
