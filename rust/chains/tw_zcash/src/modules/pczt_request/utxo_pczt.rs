// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::pczt;
use tw_bitcoin::modules::tx_builder::public_keys::PublicKeys;
use tw_bitcoin::modules::tx_builder::script_parser::{StandardScript, StandardScriptParser};
use tw_coin_entry::error::prelude::{
    MapTWError, ResultContext, SigningError, SigningErrorType, SigningResult,
};
use tw_hash::H256;
use tw_utxo::script::Script;
use tw_utxo::sighash::SighashType;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;
use tw_utxo::transaction::standard_transaction::TransactionInput;
use tw_utxo::transaction::transaction_parts::Amount;
use tw_utxo::transaction::UtxoToSign;

/// Currently, we rely on `pczt` crate to build our own [`UtxoToSign`].
pub struct UtxoPczt<'a> {
    utxo: &'a pczt::transparent::Input,
    public_keys: &'a PublicKeys,
}

impl<'a> UtxoPczt<'a> {
    pub fn new(utxo: &'a pczt::transparent::Input, public_keys: &'a PublicKeys) -> Self {
        UtxoPczt { utxo, public_keys }
    }

    pub fn build(self) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let script = Script::from(self.utxo.script_pubkey.clone());
        let builder = self.prepare_builder()?;
        self.build_utxo_with_script(builder, &script)
    }

    fn prepare_builder(&self) -> SigningResult<UtxoBuilder> {
        let prevout_hash = H256::from(self.utxo.prevout_txid);
        let prevout_index = self.utxo.prevout_index;
        let sequence = self.utxo.sequence;
        let amount: Amount = self
            .utxo
            .value
            .try_into()
            .tw_err(SigningErrorType::Error_invalid_utxo_amount)
            .context("PCZT UTXO amount is too large")?;

        // [`pczt::transparent::Input::sighash_type`] is a private field, assume it as default.
        let sighash_ty = SighashType::from_u32(self.utxo.sighash_type as u32)
            .context("Invalid sighash type in PCZT UTXO")?;

        let builder = UtxoBuilder::default()
            .prev_txid(prevout_hash)
            .prev_index(prevout_index)
            .sighash_type(sighash_ty)
            .amount(amount);

        match sequence {
            Some(seq) => Ok(builder.sequence(seq)),
            None => Ok(builder),
        }
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
            StandardScript::P2WPKH(_pubkey_hash) => {
                SigningError::err(SigningErrorType::Error_not_supported)
                    .context("ZCash does not support P2WPKH script path")
            },
            StandardScript::P2TR(_tweaked_pubkey) => {
                SigningError::err(SigningErrorType::Error_not_supported)
                    .context("ZCash does not support P2TR script path")
            },
            StandardScript::P2SH(_) | StandardScript::P2WSH(_) => {
                SigningError::err(SigningErrorType::Error_not_supported)
                    .context("P2SH and P2WSH scriptPubkeys are not supported yet")
            },
            StandardScript::OpReturn(_) => SigningError::err(SigningErrorType::Error_invalid_utxo)
                .context("Cannot spend an OP_RETURN output"),
        }
    }
}
