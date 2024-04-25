// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use lazy_static::lazy_static;

mod keypair;
mod private;
mod public;
mod signature;

pub use keypair::KeyPair;
pub use private::PrivateKey;
pub use public::PublicKey;
pub use signature::Signature;

lazy_static! {
    /// The secp256k1 engine, used to execute all signature operations including signing and verifying.
    static ref SECP256K1: secp256k1::Secp256k1<secp256k1::All> = secp256k1::Secp256k1::new();
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
    use tw_hash::sha3::keccak256;
    use tw_hash::{H256, H512};
    use tw_misc::traits::ToBytesVec;

    const SECRET: &str = "ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035";
    const MSG: &str = "71b7098e8150cde90f3ec00280815d3069f81c7cdb6d83bbe2b897b1afbe7cd6";

    fn tweak_hash() -> H256 {
        H256::try_from(keccak256(b"hello").as_slice()).unwrap()
    }

    #[test]
    fn test_sign_verify() {
        let mut key_pair = KeyPair::try_from(SECRET).unwrap();
        key_pair.no_aux_rand();

        let hash_to_sign = H256::from(MSG);

        let actual = key_pair.sign(hash_to_sign).expect("Error schnorr signing");

        let expected = H512::from("d0741ce9268e1312b41eed1a6780720ec9a9277c347ad477936b158e9e4a0effd16150cc87ee9b2efe19e3137408c62ec3e4c6a937f7825a1e1c8dfee8f1bb95");
        assert_eq!(actual, Signature::try_from(expected.as_slice()).unwrap());

        let public = key_pair.public();
        assert!(public.verify(actual, hash_to_sign), "Invalid signature");
    }

    #[test]
    fn test_sign_verify_aux_rand() {
        let private_key = PrivateKey::try_from(SECRET).unwrap();

        let hash_to_sign = H256::from(MSG);

        let actual = private_key
            .sign(hash_to_sign)
            .expect("Error schnorr signing");

        let expected_no_aux = H512::from("8c6d15c40465feef7b0b9dfa91656599efcef298cfa92742e4088bf1b5550cc2094ec9145187b219782ace4994e35e4b07f4162ffab82e9d3d1e588913908476");
        // The given signature must be different from that when the auxiliary random is disabled.
        assert_ne!(actual.to_vec(), expected_no_aux.as_slice());

        let public = private_key.public();
        assert!(public.verify(actual, hash_to_sign), "Invalid signature");
    }

    #[test]
    fn test_sign_verify_tweaked() {
        let mut tweaked_key_pair = PrivateKey::try_from(SECRET)
            .unwrap()
            .tweak(Some(tweak_hash()));
        tweaked_key_pair.no_aux_rand();

        let hash_to_sign = H256::from(MSG);

        let actual = tweaked_key_pair
            .sign(hash_to_sign)
            .expect("Error schnorr signing");

        let expected = H512::from("69e51a936bf9327d3809930bf5966c52879327bda9d6dcc6c0fd57e5f7cfebe1e403a60215cc44eacce8742125669ac63e4b33269487100e6108a5232e642bdf");
        assert_eq!(actual, Signature::try_from(expected.as_slice()).unwrap());

        let public = tweaked_key_pair.public();
        assert!(public.verify(actual, hash_to_sign), "Invalid signature");
    }

    #[test]
    fn test_sign_verify_tweaked_no_hash() {
        let mut tweaked_key_pair = PrivateKey::try_from(SECRET).unwrap().tweak(None);
        tweaked_key_pair.no_aux_rand();

        let hash_to_sign = H256::from(MSG);

        let actual = tweaked_key_pair
            .sign(hash_to_sign)
            .expect("Error schnorr signing");

        let expected = H512::from("67d56124226a9fdaf5eb654164f92cc2c2333db09a555287cba82d49a10976469dd3c2fcd6421b5a7f809b8e8ed5d3f7475e6b316e93fc31e2c36b92a97a016f");
        assert_eq!(actual, Signature::try_from(expected.as_slice()).unwrap());

        let public = tweaked_key_pair.public();
        assert!(public.verify(actual, hash_to_sign), "Invalid signature");
    }
}
