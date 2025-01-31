// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::encode::TransactionEncoder;
use crate::transaction::RippleTransaction;
use tw_coin_entry::error::prelude::SigningResult;

pub struct TransactionSigner;

impl TransactionSigner {
    pub fn sign<Transaction: RippleTransaction>(tx: Transaction) -> SigningResult<Transaction> {
        let _encoded_tx = TransactionEncoder::encode(&tx)?;
        todo!()
    }
}
