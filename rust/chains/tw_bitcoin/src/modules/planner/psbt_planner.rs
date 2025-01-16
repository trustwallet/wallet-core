// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::psbt_request::PsbtRequest;
use crate::modules::signing_request::SigningRequestBuilder;
use crate::modules::tx_builder::script_parser::StandardScriptParser;
use crate::modules::tx_builder::BitcoinChainInfo;
use std::marker::PhantomData;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_proto::BitcoinV2::Proto;
use tw_proto::BitcoinV2::Proto::mod_Input::OneOfclaiming_script as ClaimingScriptProto;
use tw_proto::BitcoinV2::Proto::mod_Output::OneOfto_recipient as ToRecipientProto;
use tw_utxo::context::UtxoContext;
use tw_utxo::transaction::standard_transaction::{TransactionInput, TransactionOutput};
use tw_utxo::transaction::transaction_interface::TransactionInterface;
use tw_utxo::transaction::UtxoToSign;

pub struct PsbtPlanner<Context: UtxoContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: UtxoContext> PsbtPlanner<Context> {
    pub fn plan_psbt(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput,
        psbt_input: &Proto::Psbt,
    ) -> SigningResult<Proto::TransactionPlan<'static>> {
        let chain_info = SigningRequestBuilder::<Context>::chain_info(coin, &input.chain_info)?;
        let PsbtRequest { unsigned_tx, .. } = PsbtRequest::<Context>::build(input, psbt_input)?;

        let total_input = unsigned_tx.total_input()?;
        let fee_estimate = unsigned_tx.fee()?;

        let vsize_estimate = unsigned_tx.estimate_transaction().vsize() as u64;

        let inputs: Vec<_> = unsigned_tx
            .input_args()
            .iter()
            .zip(unsigned_tx.inputs())
            .map(|(unsigned_txin, txin)| Self::utxo_to_proto(unsigned_txin, txin, &chain_info))
            .collect::<SigningResult<_>>()?;

        let outputs: Vec<_> = unsigned_tx
            .outputs()
            .iter()
            .map(|txout| Self::output_to_proto(txout, &chain_info))
            .collect::<SigningResult<_>>()?;

        Ok(Proto::TransactionPlan {
            inputs,
            outputs,
            available_amount: total_input,
            send_amount: total_input,
            vsize_estimate,
            fee_estimate,
            change: 0,
            ..Proto::TransactionPlan::default()
        })
    }

    pub fn utxo_to_proto(
        unsigned_txin: &UtxoToSign,
        txin: &TransactionInput,
        chain_info: &BitcoinChainInfo,
    ) -> SigningResult<Proto::Input<'static>> {
        let out_point = Proto::OutPoint {
            hash: txin.previous_output.hash.to_vec().into(),
            vout: txin.previous_output.index,
        };
        let sequence = Proto::mod_Input::Sequence {
            sequence: txin.sequence,
        };

        let from_address = StandardScriptParser
            .parse(&unsigned_txin.prevout_script_pubkey)?
            .try_to_address(chain_info)?
            .or_tw_err(SigningErrorType::Error_invalid_utxo)
            .context("Unexpected UTXO scriptPubkey")?
            .to_string();

        Ok(Proto::Input {
            out_point: Some(out_point),
            value: unsigned_txin.amount,
            sighash_type: unsigned_txin.sighash_ty.raw_sighash(),
            sequence: Some(sequence),
            claiming_script: ClaimingScriptProto::receiver_address(from_address.into()),
        })
    }

    pub fn output_to_proto(
        output: &TransactionOutput,
        chain_info: &BitcoinChainInfo,
    ) -> SigningResult<Proto::Output<'static>> {
        let to_recipient = match StandardScriptParser
            .parse(&output.script_pubkey)?
            .try_to_address(chain_info)?
        {
            Some(to_addr) => ToRecipientProto::to_address(to_addr.to_string().into()),
            // Cannot convert the output scriptPubkey into an address. Return it as is.
            None => ToRecipientProto::custom_script_pubkey(output.script_pubkey.to_vec().into()),
        };

        Ok(Proto::Output {
            value: output.value,
            to_recipient,
        })
    }
}
