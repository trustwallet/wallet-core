// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use digest::{consts::U64, Digest};

mod keypair;
mod mangle;
mod modifications;
mod private;
mod public;
mod secret;
mod signature;

pub use modifications::{cardano, waves};
pub use signature::Signature;

/// Standard `ed25519` implementation.
pub mod sha512 {
    use sha2::Sha512;

    pub type KeyPair = crate::ed25519::keypair::KeyPair<Sha512>;
    pub type PrivateKey = crate::ed25519::private::PrivateKey<Sha512>;
    pub type PublicKey = crate::ed25519::public::PublicKey<Sha512>;
}

/// `ed25519` implementation using `BLAKE2B` hash function.
pub mod blake2b {
    use blake2::Blake2b512;

    pub type KeyPair = crate::ed25519::keypair::KeyPair<Blake2b512>;
    pub type PrivateKey = crate::ed25519::private::PrivateKey<Blake2b512>;
    pub type PublicKey = crate::ed25519::public::PublicKey<Blake2b512>;
}

/// A hash function that returns 64 length output.
pub trait Hasher512: Digest<OutputSize = U64> {
    const OUTPUT_LEN: usize = 64;
    const HALF_LEN: usize = 32;
}

impl<T> Hasher512 for T where T: Digest<OutputSize = U64> {}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
    use tw_encoding::hex;
    use tw_hash::sha2::sha256;
    use tw_hash::sha3::keccak256;
    use tw_hash::{H256, H512};
    use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};

    #[test]
    fn test_private_from_bytes() {
        let secret = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";

        let private = sha512::PrivateKey::try_from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();
        let actual = private.to_zeroizing_vec();
        assert_eq!(actual.as_slice(), H256::from(secret).as_slice());
    }

    #[test]
    fn test_private_to_public() {
        let private = sha512::PrivateKey::try_from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();
        let public = private.public();

        let expected =
            H256::from("4870d56d074c50e891506d78faa4fb69ca039cc5f131eb491e166b975880e867");
        assert_eq!(public.to_vec(), expected.into_vec());
    }

    #[test]
    fn test_private_to_public_blake2b() {
        let private = blake2b::PrivateKey::try_from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();
        let public = private.public();

        let expected =
            H256::from("b689ab808542e13f3d2ec56fe1efe43a1660dcadc73ce489fde7df98dd8ce5d9");
        assert_eq!(public.to_bytes(), expected);
    }

    #[test]
    fn test_keypair_sign_verify() {
        let keypair = sha512::KeyPair::try_from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();
        let to_sign = sha256(b"Hello");
        let actual = keypair.sign(to_sign.clone()).unwrap();

        let expected = H512::from("42848abf2641a731e18b8a1fb80eff341a5acebdc56faeccdcbadb960aef775192842fccec344679446daa4d02d264259c8f9aa364164ebe0ebea218581e2e03");
        assert_eq!(actual.to_bytes(), expected);

        assert!(keypair.verify(actual, to_sign));
    }

    #[test]
    fn test_signature_malleability() {
        let orig_sign_bytes = hex::decode("ea85a47dcc18b512dfea7c209162abaea4808d77c1ec903dc7ba6e2afa3f9f07e7ed7a20a4e2fa1009db3d1443e937e6abb16ff3c3eaecb798faed7fbb40b008").unwrap();
        Signature::try_from(orig_sign_bytes.as_slice()).unwrap();

        let modified_sign_bytes = hex::decode("ea85a47dcc18b512dfea7c209162abaea4808d77c1ec903dc7ba6e2afa3f9f07d4c1707dbe450d69df7735b721e316fbabb16ff3c3eaecb798faed7fbb40b018").unwrap();
        Signature::try_from(modified_sign_bytes.as_slice()).unwrap_err();
    }

    #[test]
    fn test_keypair_sign_verify_blake2b() {
        let keypair = blake2b::KeyPair::try_from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();
        let to_sign = sha256(b"Hello");
        let actual = keypair.sign(to_sign.clone()).unwrap();

        let expected = H512::from("5c1473944cd0234ebc5a91b2966b9e707a33b936dadd149417a2e53b6b3fc97bef17b767b1690708c74d7b4c8fe48703fd44a6ef59d4cc5b9f88ba992db0a003");
        assert_eq!(actual.to_bytes(), expected);

        assert!(keypair.verify(actual, to_sign));
    }

    #[test]
    fn test_keypair_sign_verify_waves() {
        let keypair = waves::KeyPair::try_from(
            "9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a",
        )
        .unwrap();
        let to_sign = hex::decode("0402559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d00000000016372e852120000000005f5e1000000000005f5e10001570acc4110b78a6d38b34d879b5bba38806202ecf1732f8542000766616c6166656c").unwrap();
        let actual = keypair.sign(to_sign.clone()).unwrap();

        let expected = H512::from("af7989256f496e103ce95096b3f52196dd9132e044905fe486da3b829b5e403bcba95ab7e650a4a33948c2d05cfca2dce4d4df747e26402974490fb4c49fbe8f");
        assert_eq!(actual.to_bytes(), expected);

        assert!(keypair.verify(actual, to_sign));
    }

    #[test]
    fn test_private_to_public_waves() {
        let private = waves::PrivateKey::try_from(
            "9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a",
        )
        .unwrap();
        let actual = private.public();
        let expected =
            H256::from("559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d");
        assert_eq!(actual.to_vec().as_slice(), expected.as_slice());
    }

    #[test]
    fn test_private_from_bytes_cardano() {
        let secret = "b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4\
        639aadd8b6499ae39b78018b79255fbd8f585cbda9cbb9e907a72af86afb7a05d41a57c2dec9a6a19d6bf3b1fa784f334f3a0048d25ccb7b78a7b44066f9ba7bed7f28be986cbe06819165f2ee41b403678a098961013cf4a2f3e9ea61fb6c1a";

        let private = cardano::ExtendedPrivateKey::try_from(secret).unwrap();
        let actual = private.to_zeroizing_vec();
        assert_eq!(actual.as_slice(), hex::decode(secret).unwrap());
    }

    #[test]
    fn test_keypair_sign_verify_extended_cardano() {
        let secret = "b0884d248cb301edd1b34cf626ba6d880bb3ae8fd91b4696446999dc4f0b5744309941d56938e943980d11643c535e046653ca6f498c014b88f2ad9fd6e71effbf36a8fa9f5e11eb7a852c41e185e3969d518e66e6893c81d3fc7227009952d4\
        639aadd8b6499ae39b78018b79255fbd8f585cbda9cbb9e907a72af86afb7a05d41a57c2dec9a6a19d6bf3b1fa784f334f3a0048d25ccb7b78a7b44066f9ba7bed7f28be986cbe06819165f2ee41b403678a098961013cf4a2f3e9ea61fb6c1a";

        let keypair = cardano::ExtendedKeyPair::try_from(secret).unwrap();

        let message = keccak256(b"hello");
        let actual = keypair.sign(message.clone()).unwrap();

        let expected = H512::from("375df53b6a4931dcf41e062b1c64288ed4ff3307f862d5c1b1c71964ce3b14c99422d0fdfeb2807e9900a26d491d5e8a874c24f98eec141ed694d7a433a90f08");
        assert_eq!(actual.to_bytes(), expected);

        assert!(keypair.verify(actual, message));
    }

    #[test]
    fn test_public_verify_invalid_waves() {
        let invalid_sigs = [
            "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
            "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
        ];

        let public = waves::PublicKey::try_from(
            "b60076cc30ffff5c29c65af9a13ce01c3affc231d09fccbcd1277319c7911634",
        )
        .unwrap();
        let msg = vec![1, 2, 3];

        for invalid_sig_hex in invalid_sigs {
            let invalid_sig_bytes = hex::decode(invalid_sig_hex).unwrap();
            let invalid_sig = waves::Signature::try_from(invalid_sig_bytes.as_slice()).unwrap();

            public.verify(invalid_sig, msg.clone());
        }
    }

    #[test]
    fn test_private_to_public_extended_cardano() {
        let secret = "e8c8c5b2df13f3abed4e6b1609c808e08ff959d7e6fc3d849e3f2880550b574437aa559095324d78459b9bb2da069da32337e1cc5da78f48e1bd084670107f3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26fa\
        e0d152bb611cb9ff34e945e4ff627e6fba81da687a601a879759cd76530b5744424db69a75edd4780a5fbc05d1a3c84ac4166ff8e424808481dd8e77627ce5f5bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276";

        let private = cardano::ExtendedPrivateKey::try_from(secret).unwrap();
        let public = private.public();

        let expected = "fafa7eb4146220db67156a03a5f7a79c666df83eb31abbfbe77c85e06d40da3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26fa\
        f4b8d5201961e68f2e177ba594101f513ee70fe70a41324e8ea8eb787ffda6f4bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276";
        assert_eq!(public.to_vec(), hex::decode(expected).unwrap());
    }

    #[test]
    fn test_public_key_from_bytes_extended_cardano() {
        let pubkey_hex = "fafa7eb4146220db67156a03a5f7a79c666df83eb31abbfbe77c85e06d40da3110f3245ddf9132ecef98c670272ef39c03a232107733d4a1d28cb53318df26fa\
        f4b8d5201961e68f2e177ba594101f513ee70fe70a41324e8ea8eb787ffda6f4bf2eea84515a4e16c4ff06c92381822d910b5cbf9e9c144e1fb76a6291af7276";
        let pubkey_bytes = hex::decode(pubkey_hex).unwrap();

        let actual = cardano::ExtendedPublicKey::try_from(pubkey_bytes.as_slice()).unwrap();
        assert_eq!(actual.to_vec(), pubkey_bytes);
    }

    #[test]
    fn test_signature_from_bytes() {
        let signature = "418aff0000000000000000000000000000000000000000000000f600000000000000000000000000000000000000000000000000000000000000000000000010";
        let actual = Signature::try_from(hex::decode(signature).unwrap().as_slice()).unwrap();
        assert_eq!(actual.to_bytes(), H512::from(signature));
    }

    #[test]
    fn test_waves_signature_from_bytes() {
        let sig_bytes = H512::from("af7989256f496e103ce95096b3f52196dd9132e044905fe486da3b829b5e403bcba95ab7e650a4a33948c2d05cfca2dce4d4df747e26402974490fb4c49fbe8f");
        let signature = waves::Signature::try_from(sig_bytes.as_slice()).unwrap();
        assert_eq!(signature.to_vec(), sig_bytes.into_vec());
    }

    #[test]
    fn test_keypair_from_invalid_bytes() {
        let invalid = [0; 1];
        let _ = sha512::KeyPair::try_from(&invalid[..]).unwrap_err();
        let _ = sha512::PrivateKey::try_from(&invalid[..]).unwrap_err();
        let _ = sha512::PublicKey::try_from(&invalid[..]).unwrap_err();
    }

    #[test]
    fn test_debug() {
        let secret = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
        let sign = H512::from("ea85a47dcc18b512dfea7c209162abaea4808d77c1ec903dc7ba6e2afa3f9f07e7ed7a20a4e2fa1009db3d1443e937e6abb16ff3c3eaecb798faed7fbb40b008");

        let keypair = sha512::KeyPair::try_from(secret).unwrap();
        let sign = Signature::try_from(sign.as_slice()).unwrap();

        let _ = format!("{:?}", keypair);
        let _ = format!("{:?}", keypair.private());
        let _ = format!("{:?}", keypair.public());
        let _ = format!("{:?}", sign);
    }
}
