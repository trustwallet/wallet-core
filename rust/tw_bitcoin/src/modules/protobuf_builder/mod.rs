// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::script::{Script, Witness};
use tw_utxo::transaction::standard_transaction::{
    Transaction, TransactionInput, TransactionOutput,
};
use tw_utxo::transaction::transaction_interface::TransactionInterface;

pub struct ProtobufBuilder;

impl ProtobufBuilder {
    pub fn tx_to_proto(tx: &Transaction) -> Proto::Transaction<'static> {
        let inputs = tx.inputs().iter().map(Self::tx_input_to_proto).collect();
        let outputs = tx.outputs().iter().map(Self::tx_output_to_proto).collect();

        Proto::Transaction {
            version: tx.version(),
            lock_time: tx.locktime,
            inputs,
            outputs,
        }
    }

    fn tx_input_to_proto(
        input: &TransactionInput,
    ) -> Proto::mod_Transaction::TransactionInput<'static> {
        Proto::mod_Transaction::TransactionInput {
            out_point: Some(Proto::OutPoint {
                hash: Cow::from(input.previous_output.hash.to_vec()),
                vout: input.previous_output.index,
            }),
            sequence: input.sequence,
            script_sig: Self::script_data(&input.script_sig),
            witness_items: Self::witness_to_proto(&input.witness),
        }
    }

    fn tx_output_to_proto(
        output: &TransactionOutput,
    ) -> Proto::mod_Transaction::TransactionOutput<'static> {
        Proto::mod_Transaction::TransactionOutput {
            script_pubkey: Self::script_data(&output.script_pubkey),
            value: output.value,
        }
    }

    fn witness_to_proto(witness: &Witness) -> Vec<Cow<'static, [u8]>> {
        witness.as_items().iter().map(Self::script_data).collect()
    }

    fn script_data(script: &Script) -> Cow<'static, [u8]> {
        Cow::from(script.to_vec())
    }
}
