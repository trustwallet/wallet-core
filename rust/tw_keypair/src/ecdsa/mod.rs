// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use ecdsa::elliptic_curve::bigint::U256;
use ecdsa::elliptic_curve::consts::U32;
use ecdsa::elliptic_curve::CurveArithmetic;
use ecdsa::hazmat::DigestPrimitive;
use ecdsa::PrimeCurve;

mod canonical;
pub mod der;
pub mod nist256p1;
pub mod secp256k1;
pub mod signature;

/// This is an alias used for convenience.
pub trait EcdsaCurve:
    PrimeCurve<FieldBytesSize = U32, Uint = U256> + CurveArithmetic + DigestPrimitive
{
}

impl<T> EcdsaCurve for T where
    T: PrimeCurve<FieldBytesSize = U32, Uint = U256> + CurveArithmetic + DigestPrimitive
{
}
