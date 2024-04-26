// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;

pub type RegistryResult<T> = Result<T, RegistryError>;

#[derive(Debug)]
pub enum RegistryError {
    UnknownCoinType,
    Unsupported,
}

impl From<RegistryError> for SigningError {
    #[inline]
    fn from(e: RegistryError) -> Self {
        match e {
            RegistryError::UnknownCoinType => {
                SigningError::new(SigningErrorType::Error_invalid_params)
                    .context("Unknown coin type")
            },
            RegistryError::Unsupported => SigningError::new(SigningErrorType::Error_internal)
                .context("Requested coin type is not supported in Rust yet"),
        }
    }
}
