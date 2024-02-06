// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::starkex::field_element_from_bytes_be;
use crate::starkex::public::PublicKey;
use crate::starkex::signature::Signature;
use crate::traits::SigningKeyTrait;
use crate::{KeyPairError, KeyPairResult};
use starknet_crypto::{
    get_public_key, rfc6979_generate_k, sign, SignError, Signature as EcdsaSignature,
};
use starknet_ff::FieldElement;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesZeroizing;
use zeroize::Zeroizing;

/// The maximum number of attempts to sign a message.
/// As the number is coming from `rfc6979_generate_k` so the probability is lower.
const SIGN_RETRIES: usize = 5;

/// Represents a private key that is used in `starknet` context.
pub struct PrivateKey {
    secret: FieldElement,
}

impl PrivateKey {
    /// Returns an associated `starknet` public key.
    pub fn public(&self) -> PublicKey {
        let public_scalar = get_public_key(&self.secret);
        PublicKey::from_scalar(public_scalar)
    }
}

impl SigningKeyTrait for PrivateKey {
    type SigningMessage = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> KeyPairResult<Self::Signature> {
        let hash_to_sign =
            field_element_from_bytes_be(&message).map_err(|_| KeyPairError::InvalidSignMessage)?;
        let signature =
            ecdsa_sign(&self.secret, &hash_to_sign).map_err(|_| KeyPairError::SigningError)?;
        Ok(Signature::new(signature))
    }
}

impl<'a> TryFrom<&'a [u8]> for PrivateKey {
    type Error = KeyPairError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let bytes = H256::try_from(bytes).map_err(|_| KeyPairError::InvalidSecretKey)?;
        let secret = FieldElement::from_bytes_be(&bytes.take())
            .map_err(|_| KeyPairError::InvalidSecretKey)?;
        Ok(PrivateKey { secret })
    }
}

impl<'a> TryFrom<&'a str> for PrivateKey {
    type Error = KeyPairError;

    fn try_from(hex: &'a str) -> Result<Self, Self::Error> {
        let bytes = Zeroizing::new(hex::decode(hex).map_err(|_| KeyPairError::InvalidSecretKey)?);
        Self::try_from(bytes.as_slice())
    }
}

impl ToBytesZeroizing for PrivateKey {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        let secret = Zeroizing::new(self.secret.to_bytes_be());
        Zeroizing::new(secret.to_vec())
    }
}

/// `starknet-core` depends on an out-dated `starknet-crypto` crate.
/// We need to reimplement the same but using the latest `starknet-crypto` version.
/// https://github.com/xJonathanLEI/starknet-rs/blob/0c78b365c2a7a7d4138553cba42fa69d695aa73d/starknet-core/src/crypto.rs#L34-L59
pub fn ecdsa_sign(
    private_key: &FieldElement,
    message_hash: &FieldElement,
) -> Result<EcdsaSignature, SignError> {
    // Seed-retry logic ported from `cairo-lang`
    let mut seed = None;
    for _ in 0..SIGN_RETRIES {
        let k = rfc6979_generate_k(message_hash, private_key, seed.as_ref());

        match sign(private_key, message_hash, &k) {
            Ok(sig) => {
                return Ok(EcdsaSignature { r: sig.r, s: sig.s });
            },
            Err(SignError::InvalidMessageHash) => return Err(SignError::InvalidMessageHash),
            Err(SignError::InvalidK) => {
                // Bump seed and retry
                seed = match seed {
                    Some(prev_seed) => Some(prev_seed + FieldElement::ONE),
                    None => Some(FieldElement::ONE),
                };
            },
        };
    }
    Err(SignError::InvalidMessageHash)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_starknet_sign_invalid_k() {
        let private = PrivateKey {
            secret: field_element_from_bytes_be(&[0; 32]).unwrap(),
        };
        let hash = vec![0; 32];

        // Return value does not implement Debug, so we cannot unwrap here.
        match private.sign(hash) {
            Ok(_) => panic!("Retry limit expected"),
            Err(err) => assert_eq!(err, KeyPairError::SigningError),
        }
    }
}
