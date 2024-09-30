// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::address_error::AddressError;
use crate::error::prelude::*;
use tw_encoding::EncodingError;
use tw_keypair::KeyPairError;
use tw_number::NumberError;
use tw_proto::ProtoError;

impl From<NumberError> for SigningError {
    #[inline]
    fn from(_err: NumberError) -> Self {
        TWError::new(SigningErrorType::Error_invalid_params)
    }
}

impl From<AddressError> for SigningError {
    #[inline]
    fn from(_err: AddressError) -> Self {
        TWError::new(SigningErrorType::Error_invalid_address)
    }
}

impl From<serde_json::Error> for SigningError {
    fn from(_value: serde_json::Error) -> Self {
        TWError::new(SigningErrorType::Error_input_parse)
    }
}

impl From<EncodingError> for SigningError {
    fn from(_e: EncodingError) -> Self {
        TWError::new(SigningErrorType::Error_input_parse)
    }
}

impl From<KeyPairError> for SigningError {
    fn from(err: KeyPairError) -> Self {
        match err {
            KeyPairError::InvalidSecretKey => {
                TWError::new(SigningErrorType::Error_invalid_private_key)
            },
            KeyPairError::InvalidPublicKey
            | KeyPairError::InvalidSignature
            | KeyPairError::InvalidSignMessage
            | KeyPairError::SignatureVerifyError
            | KeyPairError::InvalidEncryptedMessage => {
                TWError::new(SigningErrorType::Error_invalid_params)
            },
            KeyPairError::SigningError => TWError::new(SigningErrorType::Error_signing),
            KeyPairError::InternalError => TWError::new(SigningErrorType::Error_internal),
        }
    }
}

impl From<ProtoError> for SigningError {
    fn from(_e: ProtoError) -> Self {
        TWError::new(SigningErrorType::Error_input_parse)
    }
}
