// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::starkex::field_element_from_bytes_be;
use crate::starkex::public::PublicKey;
use crate::starkex::signature::Signature;
use crate::traits::SigningKeyTrait;
use crate::Error;
use starknet_crypto::{
    get_public_key, rfc6979_generate_k, sign, SignError, Signature as EcdsaSignature,
};
use starknet_ff::FieldElement;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesZeroizing;
use zeroize::Zeroizing;

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
    type SigningHash = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, hash: Self::SigningHash) -> Result<Self::Signature, Error> {
        let hash_to_sign =
            field_element_from_bytes_be(&hash).map_err(|_| Error::InvalidSignMessage)?;
        let signature = ecdsa_sign(&self.secret, &hash_to_sign).map_err(|_| Error::SigningError)?;
        Ok(Signature::new(signature))
    }
}

impl<'a> TryFrom<&'a [u8]> for PrivateKey {
    type Error = Error;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let bytes = H256::try_from(bytes).map_err(|_| Error::InvalidSecretKey)?;
        let secret =
            FieldElement::from_bytes_be(&bytes.take()).map_err(|_| Error::InvalidSecretKey)?;
        Ok(PrivateKey { secret })
    }
}

impl From<&'static str> for PrivateKey {
    fn from(hex: &'static str) -> Self {
        // There is no need to zeroize the `data` as it has a static lifetime (so most likely included in the binary).
        let bytes = hex::decode(hex).expect("Expected a valid Private Key hex");
        PrivateKey::try_from(bytes.as_slice()).expect("Expected a valid Private Key")
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
    loop {
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
}
