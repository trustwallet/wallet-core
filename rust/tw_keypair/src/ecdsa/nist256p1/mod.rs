// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use p256::NistP256;

mod keypair;
mod private;
mod public;

pub use keypair::KeyPair;
pub use private::PrivateKey;
pub use public::PublicKey;

pub type Signature = crate::ecdsa::signature::Signature<NistP256>;
pub type VerifySignature = crate::ecdsa::signature::VerifySignature<NistP256>;

#[cfg(test)]
mod tests {
    use super::*;
    use crate::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
    use tw_encoding::hex;
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
            key_pair.public().uncompressed(),
            H520::from("046d786ab8fda678cf50f71d13641049a393b325063b8c0d4e5070de48a2caf9ab918b4fe46ccbf56701fb210d67d91c5779468f6b3fdc7a63692b9b62543f47ae")
        );
        assert_eq!(
            key_pair.public().compressed(),
            H264::from("026d786ab8fda678cf50f71d13641049a393b325063b8c0d4e5070de48a2caf9ab"),
        );
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
        let keypair = KeyPair::try_from(secret).unwrap();

        let hash_to_sign = keccak256(b"hello");
        let hash_to_sign = H256::try_from(hash_to_sign.as_slice()).unwrap();
        let signature = keypair.sign(hash_to_sign).unwrap();

        let expected = H520::from("8859e63a0c0cc2fc7f788d7e78406157b288faa6f76f76d37c4cd1534e8d83c468f9fd6ca7dde378df594625dcde98559389569e039282275e3d87c26e36447401");
        assert_eq!(signature.to_bytes(), expected);

        let verify_signature = VerifySignature::from(signature);
        assert!(keypair.verify(verify_signature, hash_to_sign));
    }

    #[test]
    fn test_public_key_from() {
        let compressed = "026d786ab8fda678cf50f71d13641049a393b325063b8c0d4e5070de48a2caf9ab";
        let uncompressed = "046d786ab8fda678cf50f71d13641049a393b325063b8c0d4e5070de48a2caf9ab918b4fe46ccbf56701fb210d67d91c5779468f6b3fdc7a63692b9b62543f47ae";
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

    // Sign(Nist256p1): msg="c449ff7fe258dc65f5ab98dbb9bfaabf5fd33535b58e46ef51452cc801bd9d2e", secret="4646464646464646464646464646464646464646464646464646464646464646"
    // Signature: 301766d925382a6ebb2ebeb18d3741954c9370dcf6d9c45b34ce7b18bc42dcdb8300d7215080efb87dd3f35de5f3b6d98aacd6161fbc0845b82d0d8be4b8b6d500
    // Sign(Nist256p1): msg="c449ff7fe258dc65f5ab98dbb9bfaabf5fd33535b58e46ef51452cc801bd9d2e", secret="4646464646464646464646464646464646464646464646464646464646464652"
    // Additional signature: 38466b25ac49a22ba8c301328ef049a61711b257987e85e25d63e0444a14e860305a4cd3bb6ea2fe80fd293abb3c592e679c42c546cbf3baa051a07b28b374a601
    //
    // Expected sign: 301766d925382a6ebb2ebeb18d3741954c9370dcf6d9c45b34ce7b18bc42dcdb7cff28ddaf7f1048822c0ca21a0c4926323a2497875b963f3b8cbd3717aa6e7c01
    // Expected additional sign: 38466b25ac49a22ba8c301328ef049a61711b257987e85e25d63e0444a14e860305a4cd3bb6ea2fe80fd293abb3c592e679c42c546cbf3baa051a07b28b374a601
    #[test]
    fn test_sign() {
        todo!()
    }
}
