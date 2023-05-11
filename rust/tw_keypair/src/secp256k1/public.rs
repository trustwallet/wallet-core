// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::secp256k1::signature::VerifySignature;
use crate::traits::VerifyingKeyTrait;
use crate::KeyPairError;
use k256::ecdsa::signature::hazmat::PrehashVerifier;
use k256::ecdsa::VerifyingKey;
use tw_encoding::hex;
use tw_hash::{H256, H264, H520};
use tw_misc::traits::ToBytesVec;

/// Represents a `secp256k1` public key.
pub struct PublicKey {
    pub(crate) public: VerifyingKey,
}

/// cbindgen:ignore
impl PublicKey {
    /// The number of bytes in a compressed public key.
    pub const COMPRESSED: usize = H264::len();
    /// The number of bytes in an uncompressed public key.
    pub const UNCOMPRESSED: usize = H520::len();

    /// Creates a public key from the given [`VerifyingKey`].
    pub(crate) fn new(public: VerifyingKey) -> PublicKey {
        PublicKey { public }
    }

    /// Returns the raw data of the compressed public key (33 bytes).
    pub fn compressed(&self) -> H264 {
        let compressed = true;
        H264::try_from(self.public.to_encoded_point(compressed).as_bytes())
            .expect("Expected 33 byte array Public Key")
    }

    /// Returns the raw data of the uncompressed public key (65 bytes).
    pub fn uncompressed(&self) -> H520 {
        let compressed = false;
        H520::try_from(self.public.to_encoded_point(compressed).as_bytes())
            .expect("Expected 65 byte array Public Key")
    }
}

impl VerifyingKeyTrait for PublicKey {
    type SigningMessage = H256;
    type VerifySignature = VerifySignature;

    fn verify(&self, sign: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        self.public
            .verify_prehash(message.as_slice(), &sign.signature)
            .is_ok()
    }
}

impl<'a> TryFrom<&'a str> for PublicKey {
    type Error = KeyPairError;

    fn try_from(hex: &'a str) -> Result<Self, Self::Error> {
        let bytes = hex::decode(hex).map_err(|_| KeyPairError::InvalidPublicKey)?;
        Self::try_from(bytes.as_slice())
    }
}

impl<'a> TryFrom<&'a [u8]> for PublicKey {
    type Error = KeyPairError;

    /// Expected either `H264` or `H520` slice.
    fn try_from(data: &'a [u8]) -> Result<Self, Self::Error> {
        Ok(PublicKey {
            public: VerifyingKey::from_sec1_bytes(data)
                .map_err(|_| KeyPairError::InvalidPublicKey)?,
        })
    }
}

/// Return the compressed bytes representation by default.
/// Consider using [`PublicKey::compressed`] or [`PublicKey::uncompressed`] instead.
impl ToBytesVec for PublicKey {
    fn to_vec(&self) -> Vec<u8> {
        self.compressed().to_vec()
    }
}
