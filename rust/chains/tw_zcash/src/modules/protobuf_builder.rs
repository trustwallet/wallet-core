// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::ZcashTransaction;
use tw_bitcoin::modules::protobuf_builder::standard_protobuf_builder::StandardProtobufBuilder;
use tw_bitcoin::modules::protobuf_builder::{ProtobufBuilder, ProtobufTransaction};
use tw_coin_entry::error::prelude::*;
use tw_proto::Zcash::Proto as ZcashProto;
use tw_utxo::context::UtxoContext;
use tw_utxo::transaction::transaction_interface::TransactionInterface;

pub struct ZcashProtobufBuilder;

impl<Context> ProtobufBuilder<Context> for ZcashProtobufBuilder
where
    Context: UtxoContext<Transaction = ZcashTransaction>,
{
    fn tx_to_proto(tx: &ZcashTransaction) -> SigningResult<ProtobufTransaction<'static>> {
        let inputs = tx
            .inputs()
            .iter()
            .map(StandardProtobufBuilder::tx_input_to_proto)
            .collect();
        let outputs = tx
            .outputs()
            .iter()
            .map(StandardProtobufBuilder::tx_output_to_proto)
            .collect::<SigningResult<Vec<_>>>()?;

        Ok(ProtobufTransaction::zcash(ZcashProto::Transaction {
            version: tx.version(),
            version_group_id: tx.version_group_id,
            inputs,
            outputs,
            lock_time: tx.locktime,
            expiry_height: tx.expiry_height,
            sapling_value_balance: tx
                .sapling_value_balance
                .try_into()
                .tw_err(SigningErrorType::Error_tx_too_big)?,
            branch_id: tx.branch_id.to_vec().into(),
        }))
    }
}
