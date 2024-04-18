// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::Common::Proto::SigningError;
use crate::EthereumAbi::Proto::AbiError;
use std::fmt;

impl fmt::Display for SigningError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let err = match self {
            SigningError::OK => "",
            SigningError::Error_general => "Unknown error",
            SigningError::Error_internal => "Internal error",
            SigningError::Error_low_balance => "Low balance: the sender balance is not enough to cover the send and other auxiliary amount such as fee, deposit, or minimal balance",
            SigningError::Error_zero_amount_requested => "Requested amount is zero, send of 0 makes no sense",
            SigningError::Error_missing_private_key => "One required key is missing (too few or wrong keys are provided)",
            SigningError::Error_invalid_private_key => "A private key provided is invalid (e.g. wrong size, usually should be 32 bytes)",
            SigningError::Error_invalid_address => "A provided address (e.g. destination address) is invalid",
            SigningError::Error_invalid_utxo => "A provided input UTXO is invalid",
            SigningError::Error_invalid_utxo_amount => "The amount of an input UTXO is invalid",
            SigningError::Error_wrong_fee => "Wrong fee is given, probably it is too low to cover minimal fee for the transaction",
            SigningError::Error_signing => "General signing error",
            SigningError::Error_tx_too_big => "Resulting transaction is too large",
            SigningError::Error_missing_input_utxos => "No input UTXOs provided",
            SigningError::Error_not_enough_utxos => "Not enough non-dust input UTXOs to cover requested amount (dust UTXOs are filtered out)",
            SigningError::Error_script_redeem => "Missing required redeem script",
            SigningError::Error_script_output => "Invalid required output script",
            SigningError::Error_script_witness_program => "Unrecognized witness program",
            SigningError::Error_invalid_memo => "Invalid memo",
            SigningError::Error_input_parse => "Some input field cannot be parsed",
            SigningError::Error_no_support_n2n => "Multi-input and multi-output transaction not supported",
            SigningError::Error_signatures_count => "Incorrect count of signatures passed to compile",
            SigningError::Error_invalid_params => "Incorrect input parameter",
            SigningError::Error_invalid_requested_token_amount => "Invalid input token amount",
            SigningError::Error_not_supported => "Operation not supported for the chain",
            SigningError::Error_dust_amount_requested => "Requested amount is too low (less dust)",
        };
        write!(f, "{err}")
    }
}

impl fmt::Display for AbiError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let err = match self {
            AbiError::OK => "",
            AbiError::Error_internal => "Internal error",
            AbiError::Error_abi_mismatch => "ABI mismatch",
            AbiError::Error_invalid_abi => "Invalid ABI provided",
            AbiError::Error_invalid_param_type => "Invalid parameter type",
            AbiError::Error_invalid_address_value => "Invalid address",
            AbiError::Error_invalid_uint_value => "Invalid Uint",
            AbiError::Error_missing_param_type => "Missing one of the parameter types",
            AbiError::Error_missing_param_value => "Missing one of the parameter values",
            AbiError::Error_decoding_data => "Error decoding data",
            AbiError::Error_empty_type => "Empty array/tuple not allowed",
        };
        write!(f, "{}", err)
    }
}
