// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::DecredContext;
use crate::modules::transaction_builder::{DecredOutputBuilder, DecredTransactionBuilder};
use crate::transaction::TRANSACTION_VERSION_1;
use tw_bitcoin::modules::signing_request::standard_signing_request::{
    chain_info, StandardSigningRequestBuilder,
};
use tw_bitcoin::modules::signing_request::SigningRequestBuilder;
use tw_bitcoin::modules::tx_builder::output_protobuf::OutputProtobuf;
use tw_bitcoin::modules::tx_builder::utxo_protobuf::UtxoProtobuf;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_proto::BitcoinV2::Proto::{SigningInput, TransactionBuilder};
use tw_utxo::modules::tx_planner::{PlanRequest, RequestType};

pub const DEFAULT_EXPIRY: u32 = 0;

pub struct DecredSigningRequestBuilder;

impl SigningRequestBuilder<DecredContext> for DecredSigningRequestBuilder {
    fn build(
        coin: &dyn CoinContext,
        input: &SigningInput,
        transaction_builder: &TransactionBuilder,
    ) -> SigningResult<PlanRequest<DecredContext>> {
        let chain_info = chain_info(coin, &input.chain_info)?;
        let dust_policy =
            StandardSigningRequestBuilder::dust_policy(&transaction_builder.dust_policy)?;
        let fee_estimator = StandardSigningRequestBuilder::fee_estimator(transaction_builder)?;
        StandardSigningRequestBuilder::expect_transaction_version(
            &transaction_builder.version,
            TRANSACTION_VERSION_1,
        )?;

        let public_keys = StandardSigningRequestBuilder::get_public_keys::<DecredContext>(input)?;

        let mut builder = DecredTransactionBuilder::new();
        builder
            .lock_time(transaction_builder.lock_time)
            .expiry(DEFAULT_EXPIRY);

        // Parse all UTXOs.
        for utxo_proto in transaction_builder.inputs.iter() {
            let utxo_builder =
                UtxoProtobuf::<DecredContext>::new(&chain_info, utxo_proto, &public_keys);
            let (utxo, utxo_args) = utxo_builder
                .utxo_from_proto()
                .context("Error creating UTXO from Protobuf")?;
            builder.push_standard_input(utxo, utxo_args)?;
        }

        // If `max_amount_output` is set, construct a transaction with only one output.
        if let Some(max_output_proto) = transaction_builder.max_amount_output.as_ref() {
            let output_builder =
                OutputProtobuf::<DecredContext>::new(&chain_info, max_output_proto);

            let max_output = output_builder
                .output_from_proto()
                .context("Error creating Max Output from Protobuf")?;
            builder.push_standard_output(max_output);

            let unsigned_tx = builder.build()?;
            return Ok(PlanRequest {
                ty: RequestType::SendMax { unsigned_tx },
                dust_policy,
                fee_estimator,
            });
        }

        // `max_amount_output` isn't set, parse all Outputs.
        for output_proto in transaction_builder.outputs.iter() {
            let output = OutputProtobuf::<DecredContext>::new(&chain_info, output_proto)
                .output_from_proto()
                .context("Error creating Output from Proto")?;
            builder.push_standard_output(output);
        }

        // Parse change output if it was provided.
        let change_output = transaction_builder
            .change_output
            .as_ref()
            .map(|change_output_proto| {
                OutputProtobuf::<DecredContext>::new(&chain_info, change_output_proto)
                    .output_from_proto()
                    .map(|out| DecredOutputBuilder::from_standard_output(out).build())
                    .context("Error creating Change Output from Proto")
            })
            .transpose()?;

        let input_selector =
            StandardSigningRequestBuilder::input_selector(&transaction_builder.input_selector);

        let unsigned_tx = builder.build()?;
        Ok(PlanRequest {
            ty: RequestType::SendExact {
                unsigned_tx,
                change_output,
                input_selector,
            },
            dust_policy,
            fee_estimator,
        })
    }
}
