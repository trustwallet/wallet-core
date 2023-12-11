// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
