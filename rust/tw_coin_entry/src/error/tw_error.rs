// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::prelude::TWErrorKind;
use crate::error::tw_result::TWResult;
use std::fmt;

/// The wrapper over the `Common::TWErrorKindType` error for convenient use.
pub struct TWError {
    error: TWErrorKind,
    context: Vec<String>,
}

impl TWError {
    pub fn new<E>(error: E) -> TWError
    where
        TWErrorKind: From<E>,
    {
        TWError {
            error: TWErrorKind::from(error),
            context: Vec::default(),
        }
    }

    pub fn err<T, E>(error: E) -> TWResult<T>
    where
        TWErrorKind: From<E>,
    {
        Err(TWError::new(error))
    }

    pub fn context<C>(mut self, context: C) -> TWError
    where
        C: fmt::Display,
    {
        self.context.push(context.to_string());
        self
    }
}

impl fmt::Display for TWError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let base_err = match self.error {
            TWErrorKind::OK => "",
            TWErrorKind::Error_general => "Unknown error",
            TWErrorKind::Error_internal => "Internal error",
            TWErrorKind::Error_low_balance => "Low balance: the sender balance is not enough to cover the send and other auxiliary amount such as fee, deposit, or minimal balance",
            TWErrorKind::Error_zero_amount_requested => "Requested amount is zero, send of 0 makes no sense",
            TWErrorKind::Error_missing_private_key => "One required key is missing (too few or wrong keys are provided)",
            TWErrorKind::Error_invalid_private_key => "A private key provided is invalid (e.g. wrong size, usually should be 32 bytes)",
            TWErrorKind::Error_invalid_address => "A provided address (e.g. destination address) is invalid",
            TWErrorKind::Error_invalid_utxo => "A provided input UTXO is invalid",
            TWErrorKind::Error_invalid_utxo_amount => "The amount of an input UTXO is invalid",
            TWErrorKind::Error_wrong_fee => "Wrong fee is given, probably it is too low to cover minimal fee for the transaction",
            TWErrorKind::Error_signing => "General signing error",
            TWErrorKind::Error_tx_too_big => "Resulting transaction is too large",
            TWErrorKind::Error_missing_input_utxos => "No input UTXOs provided",
            TWErrorKind::Error_not_enough_utxos => "Not enough non-dust input UTXOs to cover requested amount (dust UTXOs are filtered out)",
            TWErrorKind::Error_script_redeem => "Missing required redeem script",
            TWErrorKind::Error_script_output => "Invalid required output script",
            TWErrorKind::Error_script_witness_program => "Unrecognized witness program",
            TWErrorKind::Error_invalid_memo => "Invalid memo",
            TWErrorKind::Error_input_parse => "Some input field cannot be parsed",
            TWErrorKind::Error_no_support_n2n => "Multi-input and multi-output transaction not supported",
            TWErrorKind::Error_signatures_count => "Incorrect count of signatures passed to compile",
            TWErrorKind::Error_invalid_params => "Incorrect input parameter",
            TWErrorKind::Error_invalid_requested_token_amount => "Invalid input token amount",
            TWErrorKind::Error_not_supported => "Operation not supported for the chain",
            TWErrorKind::Error_dust_amount_requested => "Requested amount is too low (less dust)",
        };

        writeln!(f, "{base_err}. Context:")?;
        for (i, context) in self.context.iter().enumerate() {
            write!(f, "{i}. {context}")?;
            if i == self.context.len() - 1 {
                writeln!(f)?;
            }
        }
        Ok(())
    }
}

impl fmt::Debug for TWError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{self}")
    }
}
