// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_proto::BitcoinV3::Proto;
use tw_utxo::sighash_computer::UtxoToSign;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::standard_transaction::unsigned_transaction::UnsignedTransaction;
use tw_utxo::transaction::standard_transaction::TransactionInput;

pub mod utxo_protobuf;

/// The module is dedicated to convert [`Proto::SigningInput`] to [`UnsignedTransaction`].
pub struct TransactionProtobuf;

impl TransactionProtobuf {
    pub fn unsigned_tx_from_proto(
        _input: &Proto::SigningInput,
    ) -> SigningResult<UnsignedTransaction> {
        let _transaction_builder = TransactionBuilder::default();
        todo!()
    }

    pub fn utxo_from_proto(_input: &Proto::Input) -> SigningResult<(TransactionInput, UtxoToSign)> {
        todo!()
    }
}
