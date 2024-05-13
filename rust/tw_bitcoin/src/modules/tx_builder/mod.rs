// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_proto::BitcoinV3::Proto;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::standard_transaction::Transaction as StandardTransaction;
use tw_utxo::transaction::unsigned_transaction::UnsignedTransaction;

pub mod output_protobuf;
pub mod public_keys;
pub mod utxo_protobuf;

/// The module is dedicated to convert [`Proto::SigningInput`] to [`UnsignedTransaction`].
pub struct TransactionProtobuf;

impl TransactionProtobuf {
    pub fn unsigned_tx_from_proto(
        _input: &Proto::SigningInput,
    ) -> SigningResult<UnsignedTransaction<StandardTransaction>> {
        let _transaction_builder = TransactionBuilder::default();
        todo!()
    }
}
