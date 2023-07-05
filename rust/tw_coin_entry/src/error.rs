// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::fmt;
use std::fmt::Formatter;
use tw_keypair::KeyPairError;
use tw_number::NumberError;
use tw_proto::Common::Proto;

pub type AddressResult<T> = Result<T, AddressError>;

#[derive(Debug)]
pub enum AddressError {
    MissingPrefix,
    FromHexError,
    PublicKeyTypeMismatch,
}

pub type SigningResult<T> = Result<T, SigningError>;

/// The wrapper over the `Common::Proto::SigningError` error for convenient use.
pub struct SigningError(pub Proto::SigningError);

impl From<NumberError> for SigningError {
    fn from(_err: NumberError) -> Self {
        SigningError(Proto::SigningError::Error_invalid_params)
    }
}

impl From<AddressError> for SigningError {
    fn from(_err: AddressError) -> Self {
        SigningError(Proto::SigningError::Error_invalid_params)
    }
}

impl From<KeyPairError> for SigningError {
    fn from(err: KeyPairError) -> Self {
        match err {
            KeyPairError::InvalidSecretKey => {
                SigningError(Proto::SigningError::Error_invalid_private_key)
            },
            KeyPairError::SigningError => SigningError(Proto::SigningError::Error_signing),
            _ => SigningError(Proto::SigningError::Error_internal),
        }
    }
}

impl fmt::Display for SigningError {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        let str = match self.0 {
            Proto::SigningError::OK => "",
            Proto::SigningError::Error_general => "Unknown error",
            Proto::SigningError::Error_internal => "Internal error",
            Proto::SigningError::Error_low_balance => "Low balance: the sender balance is not enough to cover the send and other auxiliary amount such as fee, deposit, or minimal balance",
            Proto::SigningError::Error_zero_amount_requested => "Requested amount is zero, send of 0 makes no sense",
            Proto::SigningError::Error_missing_private_key => "One required key is missing (too few or wrong keys are provided)",
            Proto::SigningError::Error_invalid_private_key => "A private key provided is invalid (e.g. wrong size, usually should be 32 bytes)",
            Proto::SigningError::Error_invalid_address => "A provided address (e.g. destination address) is invalid",
            Proto::SigningError::Error_invalid_utxo => "A provided input UTXO is invalid",
            Proto::SigningError::Error_invalid_utxo_amount => "The amount of an input UTXO is invalid",
            Proto::SigningError::Error_wrong_fee => "Wrong fee is given, probably it is too low to cover minimal fee for the transaction",
            Proto::SigningError::Error_signing => "General signing error",
            Proto::SigningError::Error_tx_too_big => "Resulting transaction is too large",
            Proto::SigningError::Error_missing_input_utxos => "No input UTXOs provided",
            Proto::SigningError::Error_not_enough_utxos => "Not enough non-dust input UTXOs to cover requested amount (dust UTXOs are filtered out)",
            Proto::SigningError::Error_script_redeem => "Missing required redeem script",
            Proto::SigningError::Error_script_output => "Invalid required output script",
            Proto::SigningError::Error_script_witness_program => "Unrecognized witness program",
            Proto::SigningError::Error_invalid_memo => "Invalid memo",
            Proto::SigningError::Error_input_parse => "Some input field cannot be parsed",
            Proto::SigningError::Error_no_support_n2n => "Multi-input and multi-output transaction not supported",
            Proto::SigningError::Error_signatures_count => "Incorrect count of signatures passed to compile",
            Proto::SigningError::Error_invalid_params => "Incorrect input parameter",
            Proto::SigningError::Error_invalid_requested_token_amount => "Invalid input token amount",
        };
        write!(f, "{str}")
    }
}
