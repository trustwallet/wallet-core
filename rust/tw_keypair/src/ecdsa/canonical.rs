// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! This module is a Proof Of Concept that proves the possibility to implement the following in Rust:
//! https://github.com/trustwallet/wallet-core/blob/d9e35ec485b1366dd10509192d02d9dbb6877ab3/src/PrivateKey.cpp#L253-L282
//!
//! # Warning
//!
//! **Not production ready**

#![allow(dead_code)]

use crate::ecdsa::signature::Signature;
use crate::ecdsa::EcdsaCurve;
use crate::{KeyPairError, KeyPairResult};
use ecdsa::elliptic_curve::generic_array::ArrayLength;
use ecdsa::elliptic_curve::subtle::{Choice, ConditionallySelectable, ConstantTimeEq};
use ecdsa::elliptic_curve::{Curve, FieldBytesEncoding, PrimeField, Scalar};
use ecdsa::hazmat::{bits2field, DigestPrimitive, SignPrimitive};
use ecdsa::SigningKey;
use k256::sha2::digest::crypto_common::BlockSizeUser;
use k256::sha2::digest::{FixedOutput, FixedOutputReset};
use k256::sha2::Digest;
use rfc6979::{ByteArray, HmacDrbg};
use tw_hash::H256;

type CurveDigest<C> = <C as DigestPrimitive>::Digest;

/// Implements https://github.com/trustwallet/wallet-core/blob/d9e35ec485b1366dd10509192d02d9dbb6877ab3/src/PrivateKey.cpp#L253-L282
pub(crate) fn sign_with_canonical<C: EcdsaCurve, F>(
    signing_key: &SigningKey<C>,
    hash_to_sign: H256,
    mut is_canonical: F,
) -> KeyPairResult<Signature<C>>
where
    for<'a> F: FnMut(&'a Signature<C>) -> bool,
    Scalar<C>: SignPrimitive<C>,
    <C as Curve>::Uint: FieldBytesEncoding<C>,
{
    let priv_scalar = signing_key.as_nonzero_scalar();

    let nonce =
        bits2field::<C>(hash_to_sign.as_slice()).map_err(|_| KeyPairError::InvalidSignMessage)?;
    let entropy_input = &priv_scalar.to_repr();
    let n = &C::ORDER.encode_field_bytes();
    let additional_data = &[];

    let mut hmac_drbg = HmacDrbg::<CurveDigest<C>>::new(entropy_input, &nonce, additional_data);

    for _ in 0..10000 {
        // The `k` number is different on each iteration due to the `hmac_drbg` mutation.
        let k = generate_k::<CurveDigest<C>, _>(&mut hmac_drbg, n);
        let k_scalar = Scalar::<C>::from_repr(k).unwrap();

        let (sig, r) = priv_scalar
            .try_sign_prehashed(k_scalar, &nonce)
            .map_err(|_| KeyPairError::SigningError)?;
        let r = r.ok_or(KeyPairError::SigningError)?;

        let signature = Signature::<C>::new(sig, r);
        if is_canonical(&signature) {
            return Ok(signature);
        }
    }

    Err(KeyPairError::SigningError)
}

fn ct_eq<N: ArrayLength<u8>>(a: &ByteArray<N>, b: &ByteArray<N>) -> Choice {
    let mut ret = Choice::from(1);

    for (a, b) in a.iter().zip(b.iter()) {
        ret.conditional_assign(&Choice::from(0), !a.ct_eq(b));
    }

    ret
}

pub(crate) fn ct_lt<N: ArrayLength<u8>>(a: &ByteArray<N>, b: &ByteArray<N>) -> Choice {
    let mut borrow = 0;

    // Perform subtraction with borrow a byte-at-a-time, interpreting a
    // no-borrow condition as the less-than case
    for (&a, &b) in a.iter().zip(b.iter()).rev() {
        let c = (b as u16).wrapping_add(borrow >> (u8::BITS - 1));
        borrow = (a as u16).wrapping_sub(c) >> u8::BITS as u8;
    }

    !borrow.ct_eq(&0)
}

pub(crate) fn generate_k<D, N>(hmac_drbg: &mut HmacDrbg<D>, n: &ByteArray<N>) -> ByteArray<N>
where
    D: Digest + BlockSizeUser + FixedOutput<OutputSize = N> + FixedOutputReset,
    N: ArrayLength<u8>,
{
    loop {
        let mut k = ByteArray::<N>::default();
        hmac_drbg.fill_bytes(&mut k);

        let k_is_zero = ct_eq(&k, &ByteArray::default());
        if (!k_is_zero & ct_lt(&k, n)).into() {
            return k;
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::ecdsa::secp256k1::{PrivateKey, Signature};
    use tw_hash::H520;
    use tw_misc::traits::ToBytesVec;

    fn is_unsigned(byte: u8) -> bool {
        byte & 0x80 == 0
    }

    fn fio_is_canonical(sig: &Signature) -> bool {
        let sig = sig.to_vec();
        is_unsigned(sig[0])
            && !(sig[0] == 0 && is_unsigned(sig[1]))
            && is_unsigned(sig[32])
            && !(sig[32] == 0 && is_unsigned(sig[33]))
    }

    #[test]
    fn test_sign_canonical() {
        let private = PrivateKey::try_from(
            "ba0828d5734b65e3bcc2c51c93dfc26dd71bd666cc0273adee77d73d9a322035",
        )
        .unwrap();
        let hash_to_sign =
            H256::from("71b7098e8150cde90f3ec00280815d3069f81c7cdb6d83bbe2b897b1afbe7cd6");

        let actual = sign_with_canonical(&private.secret, hash_to_sign, fio_is_canonical).unwrap();
        let expected = H520::from("42ceeaa4a3d0ea0429ab09e4d969abd812c65ad4efef9e95e3a19cc3c41be3770ad0222dac6aa1b350cf9273fa922801d11b6142cb0fe639e2fe3fd988e5aec400");
        assert_eq!(actual.to_bytes(), expected);
    }
}
