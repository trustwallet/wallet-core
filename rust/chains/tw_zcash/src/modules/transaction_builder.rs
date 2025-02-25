// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::{
    ZcashTransaction, NU6_BRANCH_ID, TRANSACTION_VERSION_4, TRANSACTION_VERSION_GROUP_ID,
};
use tw_coin_entry::error::prelude::SigningResult;
use tw_hash::H32;
use tw_utxo::transaction::standard_transaction::{
    TransactionInput, TransactionOutput, DEFAULT_LOCKTIME,
};
use tw_utxo::transaction::transaction_parts::Amount;
use tw_utxo::transaction::unsigned_transaction::UnsignedTransaction;
use tw_utxo::transaction::UtxoToSign;

pub const DISABLE_EXPIRY: u32 = 0;
pub const ZERO_SAPLING_VALUE_BALANCE: Amount = 0;

pub struct ZcashTransactionBuilder {
    transaction: ZcashTransaction,
    utxo_args: Vec<UtxoToSign>,
}

impl ZcashTransactionBuilder {
    pub fn new() -> Self {
        ZcashTransactionBuilder {
            transaction: ZcashTransaction {
                version: TRANSACTION_VERSION_4,
                version_group_id: TRANSACTION_VERSION_GROUP_ID,
                transparent_inputs: Vec::default(),
                transparent_outputs: Vec::default(),
                locktime: DEFAULT_LOCKTIME,
                expiry_height: DISABLE_EXPIRY,
                sapling_value_balance: ZERO_SAPLING_VALUE_BALANCE,
                branch_id: NU6_BRANCH_ID,
            },
            utxo_args: Vec::default(),
        }
    }

    pub fn version(&mut self, version: u32) -> &mut Self {
        self.transaction.version = version;
        self
    }

    pub fn lock_time(&mut self, locktime: u32) -> &mut Self {
        self.transaction.locktime = locktime;
        self
    }

    pub fn expiry_height(&mut self, expiry_height: u32) -> &mut Self {
        self.transaction.expiry_height = expiry_height;
        self
    }

    pub fn branch_id(&mut self, branch_id: H32) -> &mut Self {
        self.transaction.branch_id = branch_id;
        self
    }

    pub fn push_input(&mut self, input: TransactionInput, arg: UtxoToSign) -> &mut Self {
        self.transaction.transparent_inputs.push(input);
        self.utxo_args.push(arg);
        self
    }

    pub fn push_output(&mut self, out: TransactionOutput) -> &mut Self {
        self.transaction.transparent_outputs.push(out);
        self
    }

    pub fn build(self) -> SigningResult<UnsignedTransaction<ZcashTransaction>> {
        UnsignedTransaction::new(self.transaction, self.utxo_args)
    }
}

impl Default for ZcashTransactionBuilder {
    fn default() -> Self {
        ZcashTransactionBuilder::new()
    }
}
