// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::tx_builder::public_keys::PublicKeys;
use crate::modules::tx_builder::script_parser::{StandardScript, StandardScriptParser};
use secp256k1::ThirtyTwoByteHash;
use tw_coin_entry::error::prelude::*;
use tw_hash::H256;
use tw_utxo::script::Script;
use tw_utxo::sighash::{SighashBase, SighashType};
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
        if let Some(ref witness_utxo) = self.utxo_psbt.witness_utxo {
            self.build_witness_utxo(witness_utxo)
        } else if let Some(ref non_witness_utxo) = self.utxo_psbt.non_witness_utxo {
            self.build_non_witness_utxo(non_witness_utxo)
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

        let expected_txid = non_witness_utxo.txid();
        let actual_txid = self.utxo.previous_output.txid;
        if actual_txid != expected_txid {
            return SigningError::err(SigningErrorType::Error_invalid_utxo).context(format!(
                "Txid mismatch between PSBT input and non-witness UTXO: PSBT references '{actual_txid}', but non-witness UTXO has '{expected_txid}'",
            ));
        }

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
                    .context("P2SH and P2WSH scriptPubkeys are not supported yet")
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

        if sighash_ty.base_type() != SighashBase::All || sighash_ty.anyone_can_pay() {
            return SigningError::err(SigningErrorType::Error_not_supported)
                .context("Only SIGHASH_ALL is supported for PSBT inputs");
        }

        let amount = amount
            .try_into()
            .tw_err(SigningErrorType::Error_invalid_utxo_amount)
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

#[cfg(test)]
mod tests {
    use bitcoin::consensus::Decodable;
    use std::io::Cursor;
    use tw_encoding::hex::DecodeHex;

    /// This test is to verify that the `bitcoin` crate correctly computes the `txid` for a transaction with both witness and non-witness inputs.
    #[test]
    fn test_mixed_witness_and_non_witness_txid() {
        let mut cursor = Cursor::new("01000000000102fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f0000000049483045022100fd8591c3611a07b55f509ec850534c7a9c49713c9b8fa0e844ea06c2e65e19d702205e3806676192e790bc93dd4c28e937c4bf97b15f189158ba1a30d7ecff5ee75503ffffffffef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a0100000000ffffffff02b0bf0314000000001976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac4bf00405000000001976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac00024730440220096d20c7e92f991c2bf38dc28118feb34019ae74ec1c17179b28cb041de7517402204594f46a911f24bdc7109ca192e6860ebf2f3a0087579b3c128d5ce0cd5ed4680321025476c2e83188368da1ff3e292e7acafcdb3566bb0ad253f62fc70f07aeee635700000000".decode_hex().unwrap());
        let tx = bitcoin::Transaction::consensus_decode(&mut cursor).unwrap();
        let txid = tx.txid();
        assert_eq!(
            txid.to_string(),
            "68c08a357a16b163983f7338185dc8befdf3e301e648b1cceca26a3fd33fefbd"
        );
    }
}
