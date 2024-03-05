// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::fmt::Formatter;
use tw_encoding::EncodingError;
use tw_keypair::KeyPairError;
use tw_number::NumberError;
use tw_proto::Common::Proto;
use tw_proto::ProtoError;

#[macro_export]
macro_rules! signing_output_error {
    ($output:ty, $error:expr) => {{
        let err = $error;

        let mut output = <$output>::default();
        output.error = err.0;
        output.error_message = std::borrow::Cow::from(err.to_string());

        output
    }};
}

pub type AddressResult<T> = Result<T, AddressError>;

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub enum AddressError {
    UnknownCoinType,
    Unsupported,
    MissingPrefix,
    FromHexError,
    FromBase58Error,
    FromBech32Error,
    PublicKeyTypeMismatch,
    UnexpectedAddressPrefix,
    UnexpectedHasher,
    InvalidHrp,
    InvalidRegistry,
    InvalidInput,
    InvalidChecksum,
    Internal,
}

pub type SigningResult<T> = Result<T, SigningError>;
pub type SigningErrorType = Proto::SigningError;

/// The wrapper over the `Common::SigningErrorType` error for convenient use.
#[derive(Debug)]
pub struct SigningError(pub SigningErrorType);

impl From<NumberError> for SigningError {
    #[inline]
    fn from(_err: NumberError) -> Self {
        SigningError(SigningErrorType::Error_invalid_params)
    }
}

impl From<AddressError> for SigningError {
    #[inline]
    fn from(_err: AddressError) -> Self {
        SigningError(SigningErrorType::Error_invalid_address)
    }
}

impl From<serde_json::Error> for SigningError {
    fn from(_value: serde_json::Error) -> Self {
        SigningError(SigningErrorType::Error_input_parse)
    }
}

impl From<EncodingError> for SigningError {
    fn from(_e: EncodingError) -> Self {
        SigningError(SigningErrorType::Error_input_parse)
    }
}

impl From<KeyPairError> for SigningError {
    fn from(err: KeyPairError) -> Self {
        match err {
            KeyPairError::InvalidSecretKey => {
                SigningError(SigningErrorType::Error_invalid_private_key)
            },
            KeyPairError::InvalidPublicKey
            | KeyPairError::InvalidSignature
            | KeyPairError::InvalidSignMessage
            | KeyPairError::SignatureVerifyError => {
                SigningError(SigningErrorType::Error_invalid_params)
            },
            KeyPairError::SigningError => SigningError(SigningErrorType::Error_signing),
        }
    }
}

impl From<ProtoError> for SigningError {
    fn from(_e: ProtoError) -> Self {
        SigningError(SigningErrorType::Error_input_parse)
    }
}

impl fmt::Display for SigningError {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        let str = match self.0 {
            SigningErrorType::OK => "",
            SigningErrorType::Error_general => "Unknown error",
            SigningErrorType::Error_internal => "Internal error",
            SigningErrorType::Error_low_balance => "Low balance: the sender balance is not enough to cover the send and other auxiliary amount such as fee, deposit, or minimal balance",
            SigningErrorType::Error_zero_amount_requested => "Requested amount is zero, send of 0 makes no sense",
            SigningErrorType::Error_missing_private_key => "One required key is missing (too few or wrong keys are provided)",
            SigningErrorType::Error_invalid_private_key => "A private key provided is invalid (e.g. wrong size, usually should be 32 bytes)",
            SigningErrorType::Error_invalid_address => "A provided address (e.g. destination address) is invalid",
            SigningErrorType::Error_invalid_utxo => "A provided input UTXO is invalid",
            SigningErrorType::Error_invalid_utxo_amount => "The amount of an input UTXO is invalid",
            SigningErrorType::Error_wrong_fee => "Wrong fee is given, probably it is too low to cover minimal fee for the transaction",
            SigningErrorType::Error_signing => "General signing error",
            SigningErrorType::Error_tx_too_big => "Resulting transaction is too large",
            SigningErrorType::Error_missing_input_utxos => "No input UTXOs provided",
            SigningErrorType::Error_not_enough_utxos => "Not enough non-dust input UTXOs to cover requested amount (dust UTXOs are filtered out)",
            SigningErrorType::Error_script_redeem => "Missing required redeem script",
            SigningErrorType::Error_script_output => "Invalid required output script",
            SigningErrorType::Error_script_witness_program => "Unrecognized witness program",
            SigningErrorType::Error_invalid_memo => "Invalid memo",
            SigningErrorType::Error_input_parse => "Some input field cannot be parsed",
            SigningErrorType::Error_no_support_n2n => "Multi-input and multi-output transaction not supported",
            SigningErrorType::Error_signatures_count => "Incorrect count of signatures passed to compile",
            SigningErrorType::Error_invalid_params => "Incorrect input parameter",
            SigningErrorType::Error_invalid_requested_token_amount => "Invalid input token amount",
            SigningErrorType::Error_not_supported => "Operation not supported for the chain",
            SigningErrorType::Error_dust_amount_requested => "Requested amount is too low (less dust)",
        };
        write!(f, "{str}")
    }
}
