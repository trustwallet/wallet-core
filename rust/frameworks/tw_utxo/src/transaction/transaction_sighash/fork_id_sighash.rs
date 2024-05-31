// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::transaction_interface::TransactionInterface;
use crate::transaction::transaction_sighash::witness0_sighash::Witness0Sighash;
use crate::transaction::UtxoPreimageArgs;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::SigningResult;
use tw_hash::H256;

/// `ForkSighash`is used to calculate a preimage hash of a P2PKH unspent output on Bitcoin fork chains
/// (if `sighash_ty` has `SIGHASH_FORK_ID` flag).
pub struct ForkIdSighash<Transaction: TransactionInterface> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction: TransactionInterface> ForkIdSighash<Transaction> {
    pub fn sighash_tx(tx: &Transaction, args: &UtxoPreimageArgs) -> SigningResult<H256> {
        Witness0Sighash::sighash_tx(tx, args)
    }
}
