// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::{ResultContext, SigningError, SigningErrorType, SigningResult};

/// A standard transaction is limited to 400k weight units (WU).
/// https://bitcoin.stackexchange.com/questions/35570/what-is-the-maximum-number-of-inputs-outputs-a-transaction-can-have
pub const MAX_TRANSACTION_WEIGHT: usize = 400_000;
/// We set a maximum of 10k inputs and outputs to avoid potential DoS attack leading to out of memory error.
/// This is a very high number and should be enough for any real world use case.
pub const MAX_INPUT_OUTPUT_COUNT: usize = 10_000;

/// Check the input and output count of a transaction against the maximum allowed limit.
pub fn check_max_input_output_count(
    input_count: usize,
    output_count: usize,
    includes_change_output: bool,
    includes_max_output: bool,
) -> SigningResult<()> {
    if input_count > MAX_INPUT_OUTPUT_COUNT {
        return SigningError::err(SigningErrorType::Error_tx_too_big).context(format!(
            "Too many inputs. Max allowed is {MAX_INPUT_OUTPUT_COUNT}"
        ));
    }

    let total_output_count =
        output_count + includes_change_output as usize + includes_max_output as usize;

    if total_output_count > MAX_INPUT_OUTPUT_COUNT {
        return SigningError::err(SigningErrorType::Error_tx_too_big).context(format!(
            "Too many outputs. Max allowed is {MAX_INPUT_OUTPUT_COUNT}"
        ));
    }
    Ok(())
}
