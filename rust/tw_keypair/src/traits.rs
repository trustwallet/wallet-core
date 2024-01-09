// Copyright © 2017-2024 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::KeyPairResult;
use tw_misc::traits::{FromSlice, ToBytesVec, ToBytesZeroizing};

pub trait KeyPairTrait: FromSlice + SigningKeyTrait + VerifyingKeyTrait {
    type Private: FromSlice + ToBytesZeroizing;
    type Public: FromSlice + ToBytesVec;

    /// Returns the reference to the public key.
    fn public(&self) -> &Self::Public;

    /// Returns the reference to the private key.
    fn private(&self) -> &Self::Private;
}

pub trait SigningKeyTrait {
    type SigningMessage: FromSlice;
    type Signature: ToBytesVec;

    /// Signs the given `hash` using the private key.
    fn sign(&self, message: Self::SigningMessage) -> KeyPairResult<Self::Signature>;
}

pub trait VerifyingKeyTrait {
    type SigningMessage: FromSlice;
    type VerifySignature: FromSlice;

    /// Verifies if the given `hash` was signed using the private key.
    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool;
}
