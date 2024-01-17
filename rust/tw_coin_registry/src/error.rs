// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::{SigningError, SigningErrorType};

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
            RegistryError::UnknownCoinType => SigningError(SigningErrorType::Error_invalid_params),
            RegistryError::Unsupported => SigningError(SigningErrorType::Error_internal),
        }
    }
}
