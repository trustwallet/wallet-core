// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::protobuf_builder::ProtobufBuilder;
use crate::modules::signing_request::SigningRequestBuilder;
use std::borrow::Cow;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_proto::BitcoinV3::Proto;
use tw_proto::BitcoinV3::Proto::mod_PreSigningOutput::{
    SigningMethod as ProtoSigningMethod, TaprootTweak as ProtoTaprootTweak,
};
use tw_utxo::modules::sighash_computer::{SighashComputer, TaprootTweak, TxPreimage};
use tw_utxo::modules::tx_compiler::TxCompiler;
use tw_utxo::modules::tx_planner::TxPlanner;
use tw_utxo::modules::utxo_selector::SelectResult;
use tw_utxo::signing_mode::SigningMethod;
use tw_utxo::transaction::transaction_fee::TransactionFee;

pub struct BitcoinCompiler;

impl BitcoinCompiler {
    /// Please note that [`Proto::SigningInput::public_key`] must be set.
    /// If the public key should be derived from a private key, please do it before this method is called.
    #[inline]
    pub fn preimage_hashes(input: Proto::SigningInput<'_>) -> Proto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(input)
            .unwrap_or_else(|e| signing_output_error!(Proto::PreSigningOutput, e))
    }

    fn preimage_hashes_impl(
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::PreSigningOutput<'static>> {
        let request = SigningRequestBuilder::build(&input)?;
        let SelectResult { unsigned_tx, .. } = TxPlanner::plan(request)?;

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
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Proto::SigningOutput<'static> {
        Self::compile_impl(input, signatures, public_keys)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn compile_impl(
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let request = SigningRequestBuilder::build(&input)?;
        let fee_per_vbyte = request.fee_per_vbyte;
        let SelectResult { unsigned_tx, .. } = TxPlanner::plan(request)?;

        let signed_tx = TxCompiler::compile(unsigned_tx, &signatures)?;
        let tx_proto = ProtobufBuilder::tx_to_proto(&signed_tx);

        Ok(Proto::SigningOutput {
            transaction: Some(tx_proto),
            encoded: Cow::from(signed_tx.encode_out()),
            txid: Cow::from(signed_tx.txid()),
            vsize: signed_tx.vsize() as u64,
            fee: signed_tx.fee(fee_per_vbyte)?,
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
