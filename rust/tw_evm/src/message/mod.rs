// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::{SigningError, SigningErrorType};
use tw_hash::H256;

pub mod eip191;
pub mod eip712;
pub mod signature;

pub type EthMessageBoxed = Box<dyn EthMessage>;
pub type MessageSigningResult<T> = Result<T, MessageSigningError>;

#[derive(Debug)]
pub enum MessageSigningError {
    InvalidParameterType,
    InvalidParameterValue,
    TypeValueMismatch,
    InvalidChainId,
    Internal,
}

impl From<MessageSigningError> for SigningError {
    fn from(err: MessageSigningError) -> Self {
        match err {
            MessageSigningError::InvalidParameterType
            | MessageSigningError::InvalidParameterValue
            | MessageSigningError::TypeValueMismatch
            | MessageSigningError::InvalidChainId => {
                SigningError(SigningErrorType::Error_invalid_params)
            },
            MessageSigningError::Internal => SigningError(SigningErrorType::Error_internal),
        }
    }
}

pub trait EthMessage {
    fn into_boxed(self) -> EthMessageBoxed
    where
        Self: 'static + Sized,
    {
        Box::new(self)
    }

    /// Returns hash of the message.
    fn hash(&self) -> MessageSigningResult<H256>;
}
