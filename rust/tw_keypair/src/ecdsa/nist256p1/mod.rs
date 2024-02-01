// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
    fn test_public_key_recover() {
        let sign_bytes = H520::from("8859e63a0c0cc2fc7f788d7e78406157b288faa6f76f76d37c4cd1534e8d83c468f9fd6ca7dde378df594625dcde98559389569e039282275e3d87c26e36447401");
        let sign = Signature::from_bytes(sign_bytes.as_slice()).unwrap();

        let signed_hash = keccak256(b"hello");
        let signed_hash = H256::try_from(signed_hash.as_slice()).unwrap();

        let actual = PublicKey::recover(sign, signed_hash).unwrap();
        let expected_compressed =
            H264::from("026d786ab8fda678cf50f71d13641049a393b325063b8c0d4e5070de48a2caf9ab");
        assert_eq!(actual.compressed(), expected_compressed);
    }
}
