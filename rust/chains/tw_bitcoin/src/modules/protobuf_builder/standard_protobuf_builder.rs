// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::protobuf_builder::{ProtobufBuilder, ProtobufTransaction};
use std::borrow::Cow;
use tw_proto::Utxo::Proto as UtxoProto;
use tw_utxo::context::UtxoContext;
use tw_utxo::script::{Script, Witness};
use tw_utxo::transaction::standard_transaction::{
    Transaction, TransactionInput, TransactionOutput,
};
use tw_utxo::transaction::transaction_interface::TransactionInterface;
use tw_utxo::transaction::transaction_parts::OutPoint;

pub struct StandardProtobufBuilder;

impl<Context> ProtobufBuilder<Context> for StandardProtobufBuilder
where
    Context: UtxoContext<Transaction = Transaction>,
{
    fn tx_to_proto(tx: &Context::Transaction) -> ProtobufTransaction<'static> {
        let inputs = tx.inputs().iter().map(Self::tx_input_to_proto).collect();
        let outputs = tx.outputs().iter().map(Self::tx_output_to_proto).collect();

        ProtobufTransaction::bitcoin(UtxoProto::Transaction {
            version: tx.version(),
            lock_time: tx.locktime,
            inputs,
            outputs,
        })
    }
}

impl StandardProtobufBuilder {
    pub fn tx_input_to_proto(input: &TransactionInput) -> UtxoProto::TransactionInput<'static> {
        UtxoProto::TransactionInput {
            out_point: Some(Self::out_point_to_proto(&input.previous_output)),
            sequence: input.sequence,
            script_sig: Self::script_data(&input.script_sig),
            witness_items: Self::witness_to_proto(&input.witness),
        }
    }

    pub fn tx_output_to_proto(output: &TransactionOutput) -> UtxoProto::TransactionOutput<'static> {
        UtxoProto::TransactionOutput {
            script_pubkey: Self::script_data(&output.script_pubkey),
            value: output.value,
        }
    }

    pub fn out_point_to_proto(previous_output: &OutPoint) -> UtxoProto::OutPoint<'static> {
        UtxoProto::OutPoint {
            hash: Cow::from(previous_output.hash.to_vec()),
            vout: previous_output.index,
        }
    }

    pub fn witness_to_proto(witness: &Witness) -> Vec<Cow<'static, [u8]>> {
        witness.as_items().iter().map(Self::script_data).collect()
    }

    pub fn script_data(script: &Script) -> Cow<'static, [u8]> {
        Cow::from(script.to_vec())
    }
}
