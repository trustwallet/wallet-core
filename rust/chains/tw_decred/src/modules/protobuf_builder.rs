// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::DecredContext;
use crate::transaction::{
    DecredTransaction, DecredTransactionInput, DecredTransactionOutput, SerializeType,
};
use std::borrow::Cow;
use tw_bitcoin::modules::protobuf_builder::standard_protobuf_builder::StandardProtobufBuilder;
use tw_bitcoin::modules::protobuf_builder::{ProtobufBuilder, ProtobufTransaction};
use tw_coin_entry::error::prelude::*;
use tw_proto::DecredV2::Proto as DecredProto;
use tw_utxo::transaction::transaction_interface::TransactionInterface;

pub struct DecredProtobufBuilder;

impl DecredProtobufBuilder {
    pub fn tx_input_to_proto(
        input: &DecredTransactionInput,
    ) -> SigningResult<DecredProto::TransactionInput<'static>> {
        Ok(DecredProto::TransactionInput {
            out_point: Some(DecredProto::OutPoint {
                hash: Cow::from(input.previous_output.out_point.hash.to_vec()),
                vout: input.previous_output.out_point.index,
                tree: input.previous_output.tree as u32,
            }),
            sequence: input.sequence,
            value: input
                .value_in
                .try_into()
                .tw_err(SigningErrorType::Error_invalid_utxo_amount)
                .context("Transaction Input amount cannot be negative")?,
            block_height: input.block_height,
            block_index: input.block_index,
            script_sig: StandardProtobufBuilder::script_data(&input.script_sig),
        })
    }

    pub fn tx_output_to_proto(
        output: &DecredTransactionOutput,
    ) -> SigningResult<DecredProto::TransactionOutput<'static>> {
        Ok(DecredProto::TransactionOutput {
            value: output
                .value
                .try_into()
                .tw_err(SigningErrorType::Error_invalid_requested_token_amount)
                .context("Transaction Output amount cannot be negative")?,
            version: output.version as u32,
            script: StandardProtobufBuilder::script_data(&output.script_pubkey),
        })
    }
}

impl ProtobufBuilder<DecredContext> for DecredProtobufBuilder {
    fn tx_to_proto(tx: &DecredTransaction) -> SigningResult<ProtobufTransaction<'static>> {
        let inputs = tx
            .inputs()
            .iter()
            .map(Self::tx_input_to_proto)
            .collect::<SigningResult<Vec<_>>>()?;
        let outputs = tx
            .outputs()
            .iter()
            .map(Self::tx_output_to_proto)
            .collect::<SigningResult<Vec<_>>>()?;

        Ok(ProtobufTransaction::decred(DecredProto::Transaction {
            // All serialized transactions include both prefix and witness.
            serialize_type: SerializeType::Full as u32,
            version: tx.version(),
            inputs,
            outputs,
            lock_time: tx.locktime,
            expiry: tx.expiry,
        }))
    }
}
