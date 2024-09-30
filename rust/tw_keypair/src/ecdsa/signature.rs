// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ecdsa::der;
use crate::ecdsa::EcdsaCurve;
use crate::{KeyPairError, KeyPairResult};
use ecdsa::elliptic_curve::FieldBytes;
use std::ops::{Range, RangeInclusive};
use tw_hash::{concat, H256, H512, H520};
use tw_misc::traits::ToBytesVec;

/// Represents an ECDSA signature.
#[derive(Clone, Debug, PartialEq)]
pub struct Signature<C: EcdsaCurve> {
    pub(crate) signature: ecdsa::Signature<C>,
    pub(crate) v: ecdsa::RecoveryId,
}

/// cbindgen:ignore
impl<C: EcdsaCurve> Signature<C> {
    /// The number of bytes for a serialized signature representation.
    pub const LEN: usize = 65;
    pub const R_RANGE: Range<usize> = 0..32;
    pub const S_RANGE: Range<usize> = 32..64;
    pub const RECOVERY_LAST: usize = 64;
    /// Expected signature with or without recovery byte in the end of the slice.
    pub const VERIFY_SIGNATURE_LEN_RANGE: RangeInclusive<usize> = 64..=65;

    /// Creates a `secp256k1` recoverable signature from the given [`k256::ecdsa::Signature`]
    /// and the `v` recovery byte.
    pub(crate) fn new(signature: ecdsa::Signature<C>, v: ecdsa::RecoveryId) -> Self {
        Signature { signature, v }
    }

    /// Returns the number of bytes for a serialized signature representation.
    pub const fn len() -> usize {
        Self::LEN
    }

    /// Returns an r-coordinate as 32 byte array.
    pub fn r(&self) -> H256 {
        let (r, _s) = self.signature.split_bytes();
        H256::try_from(r.as_slice()).expect("Expected 'r' 32 byte length array")
    }

    /// Returns an s-value as 32 byte array.
    pub fn s(&self) -> H256 {
        let (_, s) = self.signature.split_bytes();
        H256::try_from(s.as_slice()).expect("Expected 's' 32 byte length array")
    }

    /// Returns a recovery ID.
    pub fn v(&self) -> u8 {
        self.v.to_byte()
    }

    /// Tries to create a Signature from the serialized representation.
    pub fn from_bytes(sig: &[u8]) -> KeyPairResult<Self> {
        if sig.len() != Self::len() {
            return Err(KeyPairError::InvalidSignature);
        }

        let v = ecdsa::RecoveryId::from_byte(sig[Self::RECOVERY_LAST])
            .ok_or(KeyPairError::InvalidSignature)?;

        Ok(Signature {
            signature: Self::signature_from_slices(&sig[Self::R_RANGE], &sig[Self::S_RANGE])?,
            v,
        })
    }

    /// Tries to create a Signature from the parts.
    pub fn try_from_parts(r: H256, s: H256, v: u8) -> KeyPairResult<Self> {
        Ok(Signature {
            signature: Self::signature_from_slices(r.as_slice(), s.as_slice())?,
            v: ecdsa::RecoveryId::from_byte(v).ok_or(KeyPairError::InvalidSignature)?,
        })
    }

    /// Returns a standard binary signature representation:
    /// RSV, where R - 32 byte array, S - 32 byte array, V - 1 byte.
    pub fn to_bytes(&self) -> H520 {
        let (r, s) = self.signature.split_bytes();

        let mut dest = H520::default();
        dest[Self::R_RANGE].copy_from_slice(r.as_slice());
        dest[Self::S_RANGE].copy_from_slice(s.as_slice());
        dest[Self::RECOVERY_LAST] = self.v.to_byte();
        dest
    }

    pub fn to_der(&self) -> KeyPairResult<der::Signature> {
        der::Signature::new(self.r(), self.s())
    }

    /// # Panic
    ///
    /// `r` and `s` must be 32 byte arrays, otherwise the function panics.
    fn signature_from_slices(r: &[u8], s: &[u8]) -> KeyPairResult<ecdsa::Signature<C>> {
        let r = FieldBytes::<C>::clone_from_slice(r);
        let s = FieldBytes::<C>::clone_from_slice(s);

        ecdsa::Signature::<C>::from_scalars(r, s).map_err(|_| KeyPairError::InvalidSignature)
    }
}

impl<C: EcdsaCurve> ToBytesVec for Signature<C> {
    fn to_vec(&self) -> Vec<u8> {
        self.to_bytes().to_vec()
    }
}

impl<'a, C: EcdsaCurve> TryFrom<&'a [u8]> for Signature<C> {
    type Error = KeyPairError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        Signature::from_bytes(bytes)
    }
}

/// To verify the signature, it's enough to check `r` and `s` parts without the recovery ID.
pub struct VerifySignature<C: EcdsaCurve> {
    pub(crate) signature: ecdsa::Signature<C>,
}

impl<C: EcdsaCurve> VerifySignature<C> {
    pub fn from_der(der_signature: der::Signature) -> KeyPairResult<Self> {
        let signature = Signature::signature_from_slices(der_signature.r(), der_signature.s())
            .map_err(|_| KeyPairError::InvalidSignature)?;
        Ok(VerifySignature { signature })
    }

    /// Returns a standard binary signature representation:
    /// RS, where R - 32 byte array, S - 32 byte array.
    pub fn to_bytes(&self) -> H512 {
        let (r, s) = self.rs();
        concat(r, s)
    }

    /// Returns an ASN.1 DER encoded signature.
    pub fn to_der(&self) -> KeyPairResult<der::Signature> {
        let (r, s) = self.rs();
        der::Signature::new(r, s)
    }

    /// Returns R (32 byte array) and S (32 byte array) values.
    pub fn rs(&self) -> (H256, H256) {
        let (r, s) = self.signature.split_bytes();

        let mut r_bytes = H256::default();
        r_bytes.copy_from_slice(r.as_slice());

        let mut s_bytes = H256::default();
        s_bytes.copy_from_slice(s.as_slice());

        (r_bytes, s_bytes)
    }
}

impl<C: EcdsaCurve> ToBytesVec for VerifySignature<C> {
    fn to_vec(&self) -> Vec<u8> {
        self.to_bytes().to_vec()
    }
}

impl<'a, C: EcdsaCurve> TryFrom<&'a [u8]> for VerifySignature<C> {
    type Error = KeyPairError;

    fn try_from(sig: &'a [u8]) -> Result<Self, Self::Error> {
        if !Signature::<C>::VERIFY_SIGNATURE_LEN_RANGE.contains(&sig.len()) {
            return Err(KeyPairError::InvalidSignature);
        }

        Ok(VerifySignature {
            signature: Signature::signature_from_slices(
                &sig[Signature::<C>::R_RANGE],
                &sig[Signature::<C>::S_RANGE],
            )?,
        })
    }
}

impl<C: EcdsaCurve> From<Signature<C>> for VerifySignature<C> {
    fn from(sig: Signature<C>) -> Self {
        VerifySignature {
            signature: sig.signature,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use k256::Secp256k1;

    #[test]
    fn test_signature() {
        let sign_bytes = H520::from("d93fc9ae934d4f72db91cb149e7e84b50ca83b5a8a7b873b0fdb009546e3af47786bfaf31af61eea6471dbb1bec7d94f73fb90887e4f04d0e9b85676c47ab02a00");
        let sign = Signature::<Secp256k1>::from_bytes(sign_bytes.as_slice()).unwrap();
        assert_eq!(
            sign.r(),
            H256::from("d93fc9ae934d4f72db91cb149e7e84b50ca83b5a8a7b873b0fdb009546e3af47")
        );
        assert_eq!(
            sign.s(),
            H256::from("786bfaf31af61eea6471dbb1bec7d94f73fb90887e4f04d0e9b85676c47ab02a")
        );
        assert_eq!(sign.v(), 0);
        assert_eq!(sign.to_bytes(), sign_bytes);
    }

    #[test]
    fn test_signature_from_invalid_bytes() {
        Signature::<Secp256k1>::from_bytes(b"123").unwrap_err();
    }
}
