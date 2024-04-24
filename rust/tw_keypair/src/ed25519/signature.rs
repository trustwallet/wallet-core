// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{KeyPairError, KeyPairResult};
use curve25519_dalek::edwards::CompressedEdwardsY;
use curve25519_dalek::scalar::Scalar;
use tw_hash::{concat, H256, H512};
use tw_misc::traits::ToBytesVec;

/// Represents an `ed25519` signature.
/// Source: https://github.com/dalek-cryptography/ed25519-dalek/blob/1.0.1/src/signature.rs#L22-L53
#[allow(non_snake_case)]
#[derive(Clone, Debug)]
pub struct Signature {
    /// `R` is an `EdwardsPoint`, formed by using an hash function with
    /// 512-bits output to produce the digest of:
    ///
    /// - the nonce half of the `ExpandedSecretKey`, and
    /// - the message to be signed.
    ///
    /// This digest is then interpreted as a `Scalar` and reduced into an
    /// element in ℤ/lℤ.  The scalar is then multiplied by the distinguished
    /// basepoint to produce `R`, and `EdwardsPoint`.
    pub(crate) R: CompressedEdwardsY,

    /// `s` is a `Scalar`, formed by using an hash function with 512-bits output
    /// to produce the digest of:
    ///
    /// - the `r` portion of this `Signature`,
    /// - the `PublicKey` which should be used to verify this `Signature`, and
    /// - the message to be signed.
    ///
    /// This digest is then interpreted as a `Scalar` and reduced into an
    /// element in ℤ/lℤ.
    pub(crate) s: Scalar,
}

impl Signature {
    /// cbindgen:ignore
    pub const LEN: usize = H512::LEN;

    /// Returns the signature data (64 bytes).
    pub fn to_bytes(&self) -> H512 {
        let left = H256::from(self.R.to_bytes());
        let right = H256::from(self.s.to_bytes());
        concat(left, right)
    }
}

impl ToBytesVec for Signature {
    fn to_vec(&self) -> Vec<u8> {
        self.to_bytes().into_vec()
    }
}

impl<'a> TryFrom<&'a [u8]> for Signature {
    type Error = KeyPairError;

    /// Construct a `Signature` from a slice of bytes.
    /// Source: https://github.com/dalek-cryptography/ed25519-dalek/blob/1.0.1/src/signature.rs#L115-L190
    fn try_from(sign: &'a [u8]) -> Result<Self, Self::Error> {
        let bytes = H512::try_from(sign).map_err(|_| KeyPairError::InvalidSignature)?;

        let (lower, upper): (H256, H256) = bytes.split();

        let s = get_scalar(upper)?;
        Ok(Signature {
            R: CompressedEdwardsY(lower.take()),
            s,
        })
    }
}

/// Source: https://github.com/dalek-cryptography/ed25519-dalek/blob/1.0.1/src/signature.rs#L83-L102
fn get_scalar(bytes: H256) -> KeyPairResult<Scalar> {
    /// Equals to 240 decimal.
    const SIGNIFICANT_BITS_MASK: u8 = 0b1111_0000;

    // Since this is only used in signature deserialisation (i.e. upon
    // verification), we can do a "succeed fast" trick by checking that the most
    // significant 4 bits are unset.  If they are unset, we can succeed fast
    // because we are guaranteed that the scalar is fully reduced.  However, if
    // the 4th most significant bit is set, we must do the full reduction check,
    // as the order of the basepoint is roughly a 2^(252.5) bit number.
    //
    // This succeed-fast trick should succeed for roughly half of all scalars.
    let last_byte = bytes.last().expect("H256 is exactly 32 length");
    if last_byte & SIGNIFICANT_BITS_MASK == 0 {
        #[allow(deprecated)]
        return Ok(Scalar::from_bits(bytes.take()));
    }

    match Scalar::from_canonical_bytes(bytes.take()).into() {
        Some(x) => Ok(x),
        None => Err(KeyPairError::InvalidSignature),
    }
}
