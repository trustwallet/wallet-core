// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_hash::H256;

pub mod eip191;
pub mod eip712;
pub mod eip7702_authorization;
pub mod signature;

pub type EthMessageBoxed = Box<dyn EthMessage>;
pub type MessageSigningResult<T> = Result<T, MessageSigningError>;
pub type MessageSigningError = TWError<MessageSigningErrorKind>;

#[derive(Debug)]
pub enum MessageSigningErrorKind {
    InvalidParameterType,
    InvalidParameterValue,
    TypeValueMismatch,
    InvalidChainId,
    Internal,
}

pub fn to_signing(msg_err: MessageSigningError) -> SigningError {
    match msg_err.error_type() {
        MessageSigningErrorKind::InvalidParameterType
        | MessageSigningErrorKind::InvalidParameterValue
        | MessageSigningErrorKind::TypeValueMismatch
        | MessageSigningErrorKind::InvalidChainId => {
            SigningError::new(SigningErrorType::Error_invalid_params)
        },
        MessageSigningErrorKind::Internal => SigningError::new(SigningErrorType::Error_internal),
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
