// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;

use crate::ecdsa::nist256p1::public::PublicKey;
use crate::ecdsa::nist256p1::Signature;
use crate::traits::{DerivableKeyTrait, SigningKeyTrait};
use crate::{KeyPairError, KeyPairResult};
use ecdsa::elliptic_curve::point::AffineCoordinates;
use p256::ecdsa::{SigningKey, VerifyingKey};
use p256::elliptic_curve::sec1::ToEncodedPoint;
use p256::{AffinePoint, ProjectivePoint};
use tw_encoding::hex;
use tw_hash::{H256, H512};
use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};
use zeroize::{ZeroizeOnDrop, Zeroizing};

/// Represents a `nist256p1` private key.
#[derive(ZeroizeOnDrop, Clone)]
pub struct PrivateKey {
    pub(crate) secret: SigningKey,
}

impl PrivateKey {
    /// Returns an associated `nist256p1` public key.
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

    // See https://github.com/fioprotocol/fiojs/blob/master/src/ecc/key_private.js
    pub fn ecies_shared_key(&self, pubkey: &PublicKey) -> H512 {
        let shared_secret = diffie_hellman(&self.secret, &pubkey.public);
        let shared_secret_bytes = shared_secret.x().to_vec();
        let hash = tw_hash::sha2::sha512(shared_secret_bytes.as_slice());
        H512::try_from(hash.as_slice()).expect("Expected 64 byte array sha512 hash")
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

impl FromStr for PrivateKey {
    type Err = KeyPairError;

    fn from_str(hex: &str) -> Result<Self, Self::Err> {
        Self::try_from(hex)
    }
}

impl ToBytesVec for PrivateKey {
    fn to_vec(&self) -> Vec<u8> {
        self.secret.to_bytes().to_vec()
    }
}

impl ToBytesZeroizing for PrivateKey {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        let secret = Zeroizing::new(self.secret.to_bytes());
        Zeroizing::new(secret.as_slice().to_vec())
    }
}

impl DerivableKeyTrait for PrivateKey {
    fn derive_child(&self, other: &[u8]) -> KeyPairResult<Self> {
        let other: [u8; 32] = other
            .try_into()
            .map_err(|_| KeyPairError::InvalidSecretKey)?;

        let child_scalar =
            Option::<p256::NonZeroScalar>::from(p256::NonZeroScalar::from_repr(other.into()))
                .ok_or(KeyPairError::InternalError)?;

        let derived_scalar = self.secret.as_nonzero_scalar().as_ref() + child_scalar.as_ref();

        let secret = Option::<p256::NonZeroScalar>::from(p256::NonZeroScalar::new(derived_scalar))
            .map(Into::into)
            .ok_or(KeyPairError::InternalError)?;

        Ok(PrivateKey { secret })
    }
}
