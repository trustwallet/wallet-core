// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{KeyPairError, KeyPairResult};
use k256::FieldBytes;
use std::ops::{Range, RangeInclusive};
use tw_hash::{H256, H520};
use tw_misc::traits::ToBytesVec;

/// cbindgen:ignore
const R_RANGE: Range<usize> = 0..32;
/// cbindgen:ignore
const S_RANGE: Range<usize> = 32..64;
/// cbindgen:ignore
const RECOVERY_LAST: usize = Signature::len() - 1;
/// Expected signature with or without recovery byte in the end of the slice.
/// cbindgen:ignore
const VERIFY_SIGNATURE_LEN_RANGE: RangeInclusive<usize> = 64..=65;

/// Represents an ECDSA signature.
#[derive(Debug, PartialEq)]
pub struct Signature {
    signature: k256::ecdsa::Signature,
    v: u8,
}

/// cbindgen:ignore
impl Signature {
    /// The number of bytes for a serialized signature representation.
    pub const LEN: usize = 65;

    /// Creates a `secp256k1` recoverable signature from the given [`k256::ecdsa::Signature`]
    /// and the `v` recovery byte.
    pub(crate) fn new(signature: k256::ecdsa::Signature, v: u8) -> Signature {
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
        self.v
    }

    /// Tries to create a Signature from the serialized representation.
    pub fn from_bytes(sig: &[u8]) -> KeyPairResult<Signature> {
        if sig.len() != Signature::len() {
            return Err(KeyPairError::InvalidSignature);
        }

        Ok(Signature {
            signature: Self::signature_from_slices(&sig[R_RANGE], &sig[S_RANGE])?,
            v: sig[RECOVERY_LAST],
        })
    }

    /// Returns a standard binary signature representation:
    /// RSV, where R - 32 byte array, S - 32 byte array, V - 1 byte.
    pub fn to_bytes(&self) -> H520 {
        let (r, s) = self.signature.split_bytes();

        let mut dest = H520::default();
        dest[R_RANGE].copy_from_slice(r.as_slice());
        dest[S_RANGE].copy_from_slice(s.as_slice());
        dest[RECOVERY_LAST] = self.v;
        dest
    }

    /// # Panic
    ///
    /// `r` and `s` must be 32 byte arrays, otherwise the function panics.
    fn signature_from_slices(r: &[u8], s: &[u8]) -> KeyPairResult<k256::ecdsa::Signature> {
        let r = FieldBytes::clone_from_slice(r);
        let s = FieldBytes::clone_from_slice(s);

        k256::ecdsa::Signature::from_scalars(r, s).map_err(|_| KeyPairError::InvalidSignature)
    }
}

impl ToBytesVec for Signature {
    fn to_vec(&self) -> Vec<u8> {
        self.to_bytes().to_vec()
    }
}

/// To verify the signature, it's enough to check `r` and `s` parts without the recovery ID.
pub struct VerifySignature {
    pub signature: k256::ecdsa::Signature,
}

impl<'a> TryFrom<&'a [u8]> for VerifySignature {
    type Error = KeyPairError;

    fn try_from(sig: &'a [u8]) -> Result<Self, Self::Error> {
        if !VERIFY_SIGNATURE_LEN_RANGE.contains(&sig.len()) {
            return Err(KeyPairError::InvalidSignature);
        }

        Ok(VerifySignature {
            signature: Signature::signature_from_slices(&sig[R_RANGE], &sig[S_RANGE])?,
        })
    }
}

impl From<Signature> for VerifySignature {
    fn from(sig: Signature) -> Self {
        VerifySignature {
            signature: sig.signature,
        }
    }
}
