// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ecdsa::secp256k1::public::PublicKey;
use crate::ecdsa::secp256k1::Signature;
use crate::traits::SigningKeyTrait;
use crate::{KeyPairError, KeyPairResult};
use k256::ecdsa::{SigningKey, VerifyingKey};
use k256::elliptic_curve::sec1::ToEncodedPoint;
use k256::{AffinePoint, ProjectivePoint};
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesZeroizing;
use zeroize::{ZeroizeOnDrop, Zeroizing};

/// Represents a `secp256k1` private key.
#[derive(ZeroizeOnDrop)]
pub struct PrivateKey {
    pub(crate) secret: SigningKey,
}

impl PrivateKey {
    /// Returns an associated `secp256k1` public key.
    pub fn public(&self) -> PublicKey {
        PublicKey::new(*self.secret.verifying_key())
    }

    /// Computes an EC Diffie-Hellman secret in constant time.
    /// The method is ported from [TW::PrivateKey::getSharedKey](https://github.com/trustwallet/wallet-core/blob/830b1c5baaf90692196163999e4ee2063c5f4e49/src/PrivateKey.cpp#L175-L191).
    pub fn shared_key_hash(&self, pubkey: &PublicKey) -> H256 {
        let shared_secret = diffie_hellman(&self.secret, &pubkey.public);

        // Get a compressed shared secret (33 bytes with a tag in front).
        let compress = true;
        let shared_secret_compressed = shared_secret.to_encoded_point(compress);

        let shared_secret_hash = tw_hash::sha2::sha256(shared_secret_compressed.as_bytes());
        H256::try_from(shared_secret_hash.as_slice()).expect("Expected 32 byte array sha256 hash")
    }
}

/// This method is inspired by [elliptic_curve::ecdh::diffie_hellman](https://github.com/RustCrypto/traits/blob/f0dbe44fea56d4c17e625ababacb580fec842137/elliptic-curve/src/ecdh.rs#L60-L70)
fn diffie_hellman(private: &SigningKey, public: &VerifyingKey) -> AffinePoint {
    let public_point = ProjectivePoint::from(*public.as_affine());
    let secret_scalar = private.as_nonzero_scalar().as_ref();
    // Multiply the secret and public to get a shared secret affine point (x, y).
    (public_point * secret_scalar).to_affine()
}

impl SigningKeyTrait for PrivateKey {
    type SigningMessage = H256;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> KeyPairResult<Self::Signature> {
        let (signature, recovery_id) = self
            .secret
            .sign_prehash_recoverable(message.as_slice())
            .map_err(|_| KeyPairError::SigningError)?;
        Ok(Signature::new(signature, recovery_id))
    }
}

impl<'a> TryFrom<&'a [u8]> for PrivateKey {
    type Error = KeyPairError;

    fn try_from(data: &'a [u8]) -> Result<Self, Self::Error> {
        let secret = SigningKey::from_slice(data).map_err(|_| KeyPairError::InvalidSecretKey)?;
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
        let secret = Zeroizing::new(self.secret.to_bytes());
        Zeroizing::new(secret.as_slice().to_vec())
    }
}
