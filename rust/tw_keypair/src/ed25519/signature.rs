// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::Error;
use curve25519_dalek::edwards::CompressedEdwardsY;
use curve25519_dalek::scalar::Scalar;
use tw_hash::{concat, H256, H512};
use tw_misc::traits::ToBytesVec;

#[allow(non_snake_case)]
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
    type Error = Error;

    fn try_from(sign: &'a [u8]) -> Result<Self, Self::Error> {
        let bytes = H512::try_from(sign).map_err(|_| Error::InvalidSignature)?;

        let (lower, upper): (H256, H256) = bytes.split();

        let s = get_scalar(upper)?;
        Ok(Signature {
            R: CompressedEdwardsY(lower.take()),
            s,
        })
    }
}

fn get_scalar(bytes: H256) -> Result<Scalar, Error> {
    // The highest 3 bits must not be set.  No other checking for the
    // remaining 2^253 - 2^252 + 27742317777372353535851937790883648493
    // potential non-reduced scalars is performed.
    //
    // This is compatible with ed25519-donna and libsodium when
    // -DED25519_COMPAT is NOT specified.
    if bytes[31] & 224 != 0 {
        return Err(Error::InvalidSignature);
    }

    Ok(Scalar::from_bits(bytes.take()))
}
