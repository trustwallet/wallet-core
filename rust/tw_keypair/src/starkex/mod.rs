// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

mod keypair;
mod private;
mod public;
mod signature;

pub use keypair::KeyPair;
pub use private::PrivateKey;
pub use public::PublicKey;
pub use signature::Signature;
use starknet_ff::FieldElement;

fn field_element_from_bytes_be(bytes: &[u8]) -> Result<FieldElement, ()> {
    const FIELD_ELEMENT_LEN: usize = 32;

    if bytes.len() > FIELD_ELEMENT_LEN {
        return Err(());
    }
    let mut buffer = [0u8; FIELD_ELEMENT_LEN];
    buffer[(FIELD_ELEMENT_LEN - bytes.len())..].copy_from_slice(bytes);
    FieldElement::from_bytes_be(&buffer).map_err(|_| ())
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
    use tw_encoding::hex;
    use tw_hash::{H256, H512};
    use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};

    #[test]
    fn test_key_pair_sign_verify() {
        let keypair =
            KeyPair::try_from("0139fe4d6f02e666e86a6f58e65060f115cd3c185bd9e98bd829636931458f79")
                .unwrap();

        let hash_to_sign =
            hex::decode("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76")
                .unwrap();
        let actual = keypair.sign(hash_to_sign.clone()).unwrap();

        let expected = H512::from("061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a");
        assert_eq!(actual.to_vec(), expected.into_vec());

        assert!(keypair.verify(actual, hash_to_sign));
    }

    #[test]
    fn test_key_pair_get_private_public() {
        let privkey_bytes =
            hex::decode("058ab7989d625b1a690400dcbe6e070627adedceff7bd196e58d4791026a8afe")
                .unwrap();
        let pubkey_bytes =
            hex::decode("02a4c7332c55d6c1c510d24272d1db82878f2302f05b53bcc38695ed5f78fffd")
                .unwrap();

        let keypair = KeyPair::try_from(privkey_bytes.as_slice()).unwrap();
        assert_eq!(
            keypair.private().to_zeroizing_vec().as_slice(),
            privkey_bytes
        );
        assert_eq!(keypair.public().to_vec(), pubkey_bytes);
    }

    #[test]
    fn test_field_element_from_bytes_be_invalid() {
        let invalid_element =
            hex::decode("00058ab7989d625b1a690400dcbe6e070627adedceff7bd196e58d4791026a8afe")
                .unwrap();
        field_element_from_bytes_be(&invalid_element).unwrap_err();
    }

    #[test]
    fn test_private_key_to_from_bytes() {
        let bytes = hex::decode("058ab7989d625b1a690400dcbe6e070627adedceff7bd196e58d4791026a8afe")
            .unwrap();
        let private = PrivateKey::try_from(bytes.as_slice()).unwrap();
        assert_eq!(private.to_zeroizing_vec().as_slice(), bytes);
    }

    #[test]
    fn test_public_key_to_from_bytes() {
        let bytes = hex::decode("02a4c7332c55d6c1c510d24272d1db82878f2302f05b53bcc38695ed5f78fffd")
            .unwrap();
        let public = PublicKey::try_from(bytes.as_slice()).unwrap();
        assert_eq!(public.to_vec(), bytes);
    }

    #[test]
    fn test_signature_to_from_bytes() {
        let bytes = hex::decode("061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a").unwrap();
        let sign = Signature::try_from(bytes.as_slice()).unwrap();
        assert_eq!(sign.to_vec(), bytes);

        assert_eq!(
            sign.r(),
            H256::from("061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f")
        );
        assert_eq!(
            sign.s(),
            H256::from("04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a")
        );
    }

    // https://github.com/xJonathanLEI/starknet-rs/issues/365
    #[test]
    fn test_verify_panic() {
        let public =
            PublicKey::try_from("03ee9bffffffffff26ffffffff60ffffffffffffffffffffffffffff004accff")
                .unwrap();
        let hash = hex::decode("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76")
            .unwrap();
        let signature_bytes = hex::decode("06ffffffffffffffffffffffffffffffffffffffffffff06ffff5dffff9bffdf00ffffff9b9b9b9b9b9b9b9bbb9bff9b9bbb9bff9b9b9b9b9b9b9b9b9b9b9b33").unwrap();
        let signature = Signature::try_from(signature_bytes.as_slice()).unwrap();

        assert!(!public.verify(signature, hash));
    }
}
