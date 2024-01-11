// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ed25519::public::PublicKey as StandardPublicKey;
use crate::ed25519::signature::Signature as StandardSignature;
use crate::ed25519::Hasher512;
use crate::{KeyPairError, KeyPairResult};
use tw_hash::{H256, H512};
use tw_misc::traits::ToBytesVec;

/// cbindgen:ignore
/// Equals to 0x80.
const PUBKEY_SIGN_MASK: u8 = 0b1000_0000;

/// cbindgen:ignore
/// Equals to 127.
const DROP_SIGN_BIT_MASK: u8 = 0b0111_1111;

pub struct Signature {
    bytes: H512,
}

impl Signature {
    /// Creates a signature by mangling a standard [`StandardSignature`].
    /// Ported: https://github.com/trustwallet/wallet-core/blob/3.1.31/src/PrivateKey.cpp#L225-L230
    pub(crate) fn new_mangling<H: Hasher512>(
        standard_pubkey: &StandardPublicKey<H>,
        standard_sign: &StandardSignature,
    ) -> Signature {
        let pubkey_bytes: H256 = standard_pubkey.to_bytes();
        let sign_bit = pubkey_bytes[31] & PUBKEY_SIGN_MASK;

        let mut bytes = standard_sign.to_bytes();
        bytes[63] &= DROP_SIGN_BIT_MASK;
        bytes[63] |= sign_bit;

        Signature { bytes }
    }

    /// Returns the signature data (64 bytes).
    pub fn to_bytes(&self) -> H512 {
        self.bytes
    }

    /// Tries to convert the signature to a standard `ed25519` representation by removing the sign bit.
    /// Ported: https://github.com/trustwallet/wallet-core/blob/3.1.31/src/PublicKey.cpp#L159-L162
    pub(crate) fn to_standard_signature(&self) -> KeyPairResult<StandardSignature> {
        let mut bytes = self.bytes;
        bytes[63] &= DROP_SIGN_BIT_MASK;
        StandardSignature::try_from(bytes.as_slice())
    }

    /// Returns a public key sign bit.
    /// Either `1` or `0`.
    /// Ported: https://github.com/trustwallet/wallet-core/blob/3.1.31/src/PublicKey.cpp#L157
    pub(crate) fn get_pubkey_sign(&self) -> u8 {
        let sign_bit = self.bytes[63] & PUBKEY_SIGN_MASK;
        if sign_bit == 0 {
            0
        } else {
            1
        }
    }
}

impl<'a> TryFrom<&'a [u8]> for Signature {
    type Error = KeyPairError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let bytes = H512::try_from(bytes).map_err(|_| KeyPairError::InvalidSignature)?;
        Ok(Signature { bytes })
    }
}

impl ToBytesVec for Signature {
    fn to_vec(&self) -> Vec<u8> {
        self.bytes.into_vec()
    }
}
