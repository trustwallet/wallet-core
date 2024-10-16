// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::protobuf_builder::ProtobufBuilder;
use crate::modules::psbt_request::PsbtRequest;
use crate::modules::signing_request::SigningRequestBuilder;
use std::borrow::Cow;
use std::marker::PhantomData;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_proto::BitcoinV2::Proto;
use tw_proto::BitcoinV2::Proto::mod_PreSigningOutput::{
    SigningMethod as ProtoSigningMethod, TaprootTweak as ProtoTaprootTweak,
};
use tw_proto::BitcoinV2::Proto::mod_SigningInput::OneOftransaction as TransactionType;
use tw_utxo::context::UtxoContext;
use tw_utxo::modules::sighash_computer::{SighashComputer, TaprootTweak, TxPreimage};
use tw_utxo::modules::sighash_verifier::SighashVerifier;
use tw_utxo::modules::tx_compiler::TxCompiler;
use tw_utxo::modules::tx_planner::TxPlanner;
use tw_utxo::modules::utxo_selector::SelectResult;
use tw_utxo::signing_mode::SigningMethod;
use tw_utxo::transaction::transaction_interface::TransactionInterface;

pub struct BitcoinCompiler<Context: UtxoContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: UtxoContext> BitcoinCompiler<Context> {
    /// Please note that [`Proto::SigningInput::public_key`] must be set.
    /// If the public key should be derived from a private key, please do it before this method is called.
    #[inline]
    pub fn preimage_hashes(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::PreSigningOutput, e))
    }

    fn preimage_hashes_impl(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::PreSigningOutput<'static>> {
        let unsigned_tx = match input.transaction {
            TransactionType::builder(ref tx_builder) => {
                let request = SigningRequestBuilder::<Context>::build(coin, &input, tx_builder)?;
                TxPlanner::plan(request)?.unsigned_tx
            },
            TransactionType::psbt(ref psbt) => {
                PsbtRequest::<Context>::build(&input, psbt)?.unsigned_tx
            },
            TransactionType::None => {
                return SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("Either `TransactionBuilder` or `Psbt` should be set")
            },
        };

        let TxPreimage { sighashes } = SighashComputer::preimage_tx(&unsigned_tx)?;

        let sighashes: Vec<_> = sighashes
            .into_iter()
            .map(|sighash| Proto::mod_PreSigningOutput::Sighash {
                public_key: Cow::from(sighash.signer_pubkey),
                sighash: Cow::from(sighash.sighash.to_vec()),
                signing_method: signing_method(sighash.signing_method),
                tweak: taproot_tweak(sighash.taproot_tweak),
            })
            .collect();

        Ok(Proto::PreSigningOutput {
            sighashes,
            ..Proto::PreSigningOutput::default()
        })
    }

    #[inline]
    pub fn compile(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Proto::SigningOutput<'static> {
        Self::compile_impl(coin, input, signatures, public_keys)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn compile_impl(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        match input.transaction {
            TransactionType::builder(ref tx) => {
                Self::compile_with_tx_builder(coin, &input, tx, signatures)
            },
            TransactionType::psbt(ref psbt) => Self::compile_psbt(coin, &input, psbt, signatures),
            TransactionType::None => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No transaction type specified"),
        }
    }

    fn compile_with_tx_builder(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput,
        tx_builder_input: &Proto::TransactionBuilder,
        signatures: Vec<SignatureBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let request = SigningRequestBuilder::<Context>::build(coin, input, tx_builder_input)?;
        let SelectResult { unsigned_tx, plan } = TxPlanner::plan(request)?;

        SighashVerifier::verify_signatures(&unsigned_tx, &signatures)?;
        let signed_tx = TxCompiler::compile(unsigned_tx, &signatures)?;
        let tx_proto = ProtobufBuilder::tx_to_proto(&signed_tx);

        Ok(Proto::SigningOutput {
            transaction: Some(tx_proto),
            encoded: Cow::from(signed_tx.encode_out()),
            txid: Cow::from(signed_tx.txid()),
            // `vsize` could have been changed after the transaction being signed.
            vsize: signed_tx.vsize() as u64,
            weight: signed_tx.weight() as u64,
            // `fee` should haven't been changed since it's a difference between `sum(inputs)` and `sum(outputs)`.
            fee: plan.fee_estimate,
            ..Proto::SigningOutput::default()
        })
    }

    fn compile_psbt(
        _coin: &dyn CoinContext,
        input: &Proto::SigningInput,
        psbt: &Proto::Psbt,
        signatures: Vec<SignatureBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let PsbtRequest { unsigned_tx, .. } = PsbtRequest::<Context>::build(input, psbt)?;
        let fee = unsigned_tx.fee()?;

        SighashVerifier::verify_signatures(&unsigned_tx, &signatures)?;
        let signed_tx = TxCompiler::compile(unsigned_tx, &signatures)?;
        let tx_proto = ProtobufBuilder::tx_to_proto(&signed_tx);

        Ok(Proto::SigningOutput {
            transaction: Some(tx_proto),
            encoded: Cow::from(signed_tx.encode_out()),
            txid: Cow::from(signed_tx.txid()),
            // `vsize` could have been changed after the transaction being signed.
            vsize: signed_tx.vsize() as u64,
            weight: signed_tx.weight() as u64,
            fee,
            ..Proto::SigningOutput::default()
        })
    }
}

pub fn signing_method(s: SigningMethod) -> ProtoSigningMethod {
    match s {
        SigningMethod::Legacy => ProtoSigningMethod::Legacy,
        SigningMethod::Segwit => ProtoSigningMethod::Segwit,
        SigningMethod::Taproot => ProtoSigningMethod::Taproot,
    }
}

pub fn taproot_tweak(tweak: Option<TaprootTweak>) -> Option<ProtoTaprootTweak<'static>> {
    tweak.map(|tweak| {
        let merkle_root = match tweak.merkle_root {
            Some(root) => root.to_vec(),
            None => Vec::default(),
        };
        ProtoTaprootTweak {
            merkle_root: Cow::from(merkle_root),
        }
    })
}
