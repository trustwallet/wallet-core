// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::sighash_computer::TxSigningArgs;
use crate::transaction::standard_transaction::Transaction;

pub struct UnsignedTransaction {
    pub transaction: Transaction,
    pub args: TxSigningArgs,
}
