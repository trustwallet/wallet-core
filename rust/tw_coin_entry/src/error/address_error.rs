// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use strum_macros::Display;

pub type AddressResult<T> = Result<T, AddressError>;

#[derive(Copy, Clone, Debug, Display, Eq, PartialEq)]
pub enum AddressError {
    UnknownCoinType,
    Unsupported,
    MissingPrefix,
    FromHexError,
    FromBase58Error,
    FromBase64Error,
    FromBech32Error,
    PublicKeyTypeMismatch,
    UnexpectedAddressPrefix,
    UnexpectedHasher,
    InvalidHrp,
    InvalidRegistry,
    InvalidInput,
    InvalidChecksum,
    InvalidWitnessProgram,
    Internal,
}
