// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::{DecredTransactionOutput, STANDARD_SCRIPT_VERSION};
use tw_utxo::transaction::standard_transaction::TransactionOutput;

pub struct DecredOutputBuilder {
    output: DecredTransactionOutput,
}

impl DecredOutputBuilder {
    pub fn from_standard_output(standard_output: TransactionOutput) -> Self {
        DecredOutputBuilder {
            output: DecredTransactionOutput {
                value: standard_output.value,
                version: STANDARD_SCRIPT_VERSION,
                script_pubkey: standard_output.script_pubkey,
            },
        }
    }

    pub fn build(self) -> DecredTransactionOutput {
        self.output
    }
}
