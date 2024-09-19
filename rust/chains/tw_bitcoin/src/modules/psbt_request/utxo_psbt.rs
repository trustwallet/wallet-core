// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::tx_builder::public_keys::PublicKeys;
use crate::modules::tx_builder::script_parser::{StandardScript, StandardScriptParser};
use secp256k1::ThirtyTwoByteHash;
use tw_coin_entry::error::prelude::*;
use tw_hash::H256;
use tw_utxo::script::Script;
use tw_utxo::sighash::SighashType;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;
use tw_utxo::transaction::standard_transaction::TransactionInput;
use tw_utxo::transaction::UtxoToSign;

/// Currently, we rely on `bitcoin` crate to build our own [`UtxoToSign`].
pub struct UtxoPsbt<'a> {
    utxo: &'a bitcoin::TxIn,
    utxo_psbt: &'a bitcoin::psbt::Input,
    public_keys: &'a PublicKeys,
}

impl<'a> UtxoPsbt<'a> {
    pub fn new(
        utxo: &'a bitcoin::TxIn,
        utxo_psbt: &'a bitcoin::psbt::Input,
        public_keys: &'a PublicKeys,
    ) -> Self {
        UtxoPsbt {
            utxo,
            utxo_psbt,
            public_keys,
        }
    }

    pub fn build(self) -> SigningResult<(TransactionInput, UtxoToSign)> {
        if let Some(ref non_witness_utxo) = self.utxo_psbt.non_witness_utxo {
            self.build_non_witness_utxo(non_witness_utxo)
        } else if let Some(ref witness_utxo) = self.utxo_psbt.witness_utxo {
            self.build_witness_utxo(witness_utxo)
        } else {
            SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Neither 'witness_utxo' nor 'non_witness_utxo' are set in the PSBT")
        }
    }

    pub fn build_non_witness_utxo(
        &self,
        non_witness_utxo: &bitcoin::Transaction,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let prev_out_idx = self.utxo.previous_output.vout as usize;
        let prev_out = non_witness_utxo
            .output
            .get(prev_out_idx)
            .or_tw_err(SigningErrorType::Error_invalid_utxo)
            .with_context(|| {
                format!("'Psbt::non_witness_utxo' does not contain '{prev_out_idx}' output")
            })?;

        let script = Script::from(prev_out.script_pubkey.to_bytes());
        let builder = self.prepare_builder(prev_out.value)?;

        self.build_utxo_with_script(builder, &script)
    }

    pub fn build_witness_utxo(
        &self,
        witness_utxo: &bitcoin::TxOut,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let script = Script::from(witness_utxo.script_pubkey.to_bytes());
        let builder = self.prepare_builder(witness_utxo.value)?;
        self.build_utxo_with_script(builder, &script)
    }

    fn build_utxo_with_script(
        &self,
        builder: UtxoBuilder,
        script: &Script,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        match StandardScriptParser.parse(script)? {
            StandardScript::P2PK(pubkey) => builder.p2pk(&pubkey),
            StandardScript::P2PKH(pubkey_hash) => {
                let pubkey = self.public_keys.get_ecdsa_public_key(&pubkey_hash)?;
                builder.p2pkh(&pubkey)
            },
            StandardScript::P2WPKH(pubkey_hash) => {
                let pubkey = self.public_keys.get_ecdsa_public_key(&pubkey_hash)?;
                builder.p2wpkh(&pubkey)
            },
            StandardScript::P2TR(tweaked_pubkey) => {
                if self.has_tap_scripts() {
                    return SigningError::err(SigningErrorType::Error_not_supported)
                        .context("P2TR script path is not supported for PSBT at the moment");
                }
                builder.p2tr_key_path_with_tweaked_pubkey(&tweaked_pubkey)
            },
            StandardScript::P2SH(_) | StandardScript::P2WSH(_) => {
                SigningError::err(SigningErrorType::Error_not_supported)
                    .context("P2SH and P2WSH scriptPubkey's are not supported yet")
            },
            StandardScript::OpReturn(_) => SigningError::err(SigningErrorType::Error_invalid_utxo)
                .context("Cannot spend an OP_RETURN output"),
        }
    }

    fn prepare_builder(&self, amount: u64) -> SigningResult<UtxoBuilder> {
        let prevout_hash = H256::from(self.utxo.previous_output.txid.to_raw_hash().into_32());
        let prevout_index = self.utxo.previous_output.vout;
        let sequence = self.utxo.sequence.0;

        let sighash_ty = match self.utxo_psbt.sighash_type {
            Some(psbt_ty) => SighashType::from_u32(psbt_ty.to_u32())?,
            None => SighashType::default(),
        };

        let amount = amount
            .try_into()
            .tw_err(|_| SigningErrorType::Error_invalid_utxo_amount)
            .context("PSBT UTXO amount is too large")?;

        Ok(UtxoBuilder::default()
            .prev_txid(prevout_hash)
            .prev_index(prevout_index)
            .sequence(sequence)
            .sighash_type(sighash_ty)
            .amount(amount))
    }

    fn has_tap_scripts(&self) -> bool {
        !self.utxo_psbt.tap_scripts.is_empty()
    }
}
