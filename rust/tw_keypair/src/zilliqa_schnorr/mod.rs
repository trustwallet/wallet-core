mod private;
mod public;
mod signature;

pub use private::PrivateKey;
pub use public::PublicKey;
pub use signature::Signature;

#[cfg(test)]
mod tests {
    use super::*;
    use crate::traits::{SigningKeyTrait, VerifyingKeyTrait};
    use crate::zilliqa_schnorr::public::VerifySignature;
    use std::str::FromStr;
    use tw_encoding::hex;
    use tw_hash::sha2::sha256;
    use tw_hash::sha3::keccak256;
    use tw_hash::H256;
    use tw_misc::traits::ToBytesVec;

    #[test]
    fn test_public_key() {
        let private_key = PrivateKey::from_str(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();

        let public_key = private_key.public();
        assert_eq!(
            hex::encode(public_key.to_vec(), false),
            "0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1"
        );
    }

    #[test]
    fn test_sign_zilliqa() {
        let private_key = PrivateKey::from_str(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();

        let message = "hello schnorr";
        let message_data = message.as_bytes();
        let digest = sha256(message_data);

        let signature = private_key.sign(digest.to_vec()).unwrap();

        assert_eq!(
            hex::encode(signature.to_vec(), false),
            "b8118ccb99563fe014279c957b0a9d563c1666e00367e9896fe541765246964f64a53052513da4e6dc20fdaf69ef0d95b4ca51c87ad3478986cf053c2dd0b853"
        );
    }

    #[test]
    fn test_verify_zilliqa() {
        let private_key = PrivateKey::from_str(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();

        let public_key = private_key.public();

        let message = "hello schnorr";
        let message_data = message.as_bytes();
        let digest = sha256(message_data);

        let signature = private_key.sign(digest.to_vec()).unwrap();

        assert_eq!(
            hex::encode(signature.to_vec(), false),
            "b8118ccb99563fe014279c957b0a9d563c1666e00367e9896fe541765246964f64a53052513da4e6dc20fdaf69ef0d95b4ca51c87ad3478986cf053c2dd0b853"
        );

        let is_valid = public_key.verify(signature, digest.to_vec());
        assert!(is_valid);
    }

    #[test]
    fn test_verify_invalid() {
        let private_key = PrivateKey::from_str(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();

        let signature_bytes  = hex::decode("b8118ccb99563fe014279c957b0a9d563c1666e00367e9896fe541765246964f64a53052513da4e6dc20fdaf69ef0d95b4ca51c87ad3478986cf053c2dd0b853").unwrap();
        let verify_sig = VerifySignature::try_from(signature_bytes.as_slice()).unwrap();

        let hash_to_sign = keccak256(b"hello");
        let hash_to_sign = H256::try_from(hash_to_sign.as_slice()).unwrap();

        assert!(!private_key
            .public()
            .verify(verify_sig, hash_to_sign.to_vec()));
    }
}
