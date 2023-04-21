// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::secp256k1::public::PublicKey;
use crate::secp256k1::signature::Signature;
use crate::traits::SigningKeyTrait;
use crate::Error;
use k256::ecdsa::SigningKey;
use tw_hash::H256;
use tw_utils::traits::ToBytesVec;

pub struct PrivateKey {
    pub(crate) secret: SigningKey,
}

impl PrivateKey {
    pub fn to_bytes(&self) -> H256 {
        H256::try_from(self.secret.to_bytes().as_slice())
            .expect("'PrivateKey::secret' is 32 byte length array")
    }

    pub fn public(&self) -> PublicKey {
        PublicKey::new(*self.secret.verifying_key())
    }

    // TODO try to figure out what tag (CompressedEvenY or CompressedOddY) should be pushed front.
    // pub fn shared_key_hash(&self, public: &PublicKey) -> H256 {
    //     use k256::ecdh::{diffie_hellman, SharedSecret};
    //     use k256::elliptic_curve::sec1::Tag;
    //     use k256::{AffinePoint, EncodedPoint};
    //
    //     let public_raw = public.compressed();
    //     let public_point = EncodedPoint::from_bytes(public_raw.as_slice())
    //         .expect("Expected valid 'k256::EncodedPoint'");
    //     let public_affine =
    //         AffinePoint::try_from(public_point).expect("Expected valid 'AffinePoint'");
    //
    //     let secret: SharedSecret = diffie_hellman(self.secret.as_nonzero_scalar(), public_affine);
    //
    //     // `SharedSecret` is 32 byte array. We need to push the `compress` tag front.
    //     let mut secret_tagged = Vec::with_capacity(33);
    //     secret_tagged.push(Tag::CompressedEvenY as u8);
    //     secret_tagged.extend_from_slice(secret.raw_secret_bytes().as_slice());
    //
    //     let secret_hash = tw_hash::sha2::sha256(&secret_tagged);
    //     H256::try_from(secret_hash.as_slice()).expect("Expected 32 byte array sha256 hash")
    // }
}

impl SigningKeyTrait for PrivateKey {
    type SigningHash = H256;
    type Signature = Signature;

    fn sign(&self, hash: Self::SigningHash) -> Result<Self::Signature, Error> {
        let (signature, recovery_id) = self
            .secret
            .sign_prehash_recoverable(&hash)
            .map_err(|_| Error::SigningError)?;
        Ok(Signature::new(signature, recovery_id.to_byte()))
    }
}

/// Implement `str` -> `PrivateKey` conversion for test purposes.
impl From<&'static str> for PrivateKey {
    fn from(hex: &'static str) -> Self {
        let data = H256::from(hex);
        PrivateKey::try_from(data.as_slice()).expect("Expected valid Private Key hex")
    }
}

impl<'a> TryFrom<&'a [u8]> for PrivateKey {
    type Error = Error;

    fn try_from(data: &'a [u8]) -> Result<Self, Self::Error> {
        let secret = SigningKey::from_slice(data).map_err(|_| Error::InvalidSecretKey)?;
        Ok(PrivateKey { secret })
    }
}

impl ToBytesVec for PrivateKey {
    fn to_vec(&self) -> Vec<u8> {
        self.to_bytes().into_vec()
    }
}
