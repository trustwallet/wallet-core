// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::Error;
use tw_utils::traits::ToBytesVec;

pub trait KeyPairTrait: FromSlice + SigningKeyTrait + VerifyingKeyTrait {
    type Private: FromSlice + ToBytesVec;
    type Public: FromSlice + ToBytesVec;

    fn public(&self) -> &Self::Public;

    fn private(&self) -> &Self::Private;
}

pub trait SigningKeyTrait {
    type SigningHash;
    type Signature: ToBytesVec;

    fn sign(&self, hash: Self::SigningHash) -> Result<Self::Signature, Error>;
}

pub trait VerifyingKeyTrait {
    type SigningHash;
    type VerifySignature: FromSlice;

    fn verify(&self, signature: Self::VerifySignature, hash: Self::SigningHash) -> bool;
}

pub trait FromSlice: for<'a> TryFrom<&'a [u8], Error = Error> {}

impl<T> FromSlice for T where for<'a> T: TryFrom<&'a [u8], Error = Error> {}
