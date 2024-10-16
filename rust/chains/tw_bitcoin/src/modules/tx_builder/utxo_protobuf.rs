// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::tx_builder::public_keys::PublicKeys;
use crate::modules::tx_builder::script_parser::{StandardScript, StandardScriptParser};
use crate::modules::tx_builder::BitcoinChainInfo;
use std::marker::PhantomData;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::{H160, H256};
use tw_keypair::{ecdsa, schnorr};
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::context::UtxoContext;
use tw_utxo::script::Script;
use tw_utxo::sighash::SighashType;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;
use tw_utxo::transaction::standard_transaction::TransactionInput;
use tw_utxo::transaction::transaction_parts::OutPoint;
use tw_utxo::transaction::UtxoToSign;

pub struct UtxoProtobuf<'a, Context: UtxoContext> {
    chain_info: &'a BitcoinChainInfo,
    input: &'a Proto::Input<'a>,
    public_keys: &'a PublicKeys,
    _phantom: PhantomData<Context>,
}

impl<'a, Context: UtxoContext> UtxoProtobuf<'a, Context> {
    pub fn new(
        chain_info: &'a BitcoinChainInfo,
        input: &'a Proto::Input<'a>,
        public_keys: &'a PublicKeys,
    ) -> Self {
        UtxoProtobuf {
            chain_info,
            input,
            public_keys,
            _phantom: PhantomData,
        }
    }

    pub fn utxo_from_proto(self) -> SigningResult<(TransactionInput, UtxoToSign)> {
        use Proto::mod_Input::mod_InputBuilder::OneOfvariant as BuilderType;
        use Proto::mod_Input::OneOfclaiming_script as ScriptType;

        match self.input.claiming_script {
            ScriptType::script_builder(ref builder) => match builder.variant {
                // BuilderType::p2sh(ref redeem_script) => self.p2sh(redeem_script.to_vec()),
                BuilderType::p2pk(ref pubkey) => self.p2pk(pubkey),
                BuilderType::p2pkh(ref pubkey_or_hash) => self.p2pkh(pubkey_or_hash),
                // BuilderType::p2wsh(ref redeem_script) => self.p2wsh(redeem_script.to_vec()),
                BuilderType::p2wpkh(ref pubkey_or_hash) => self.p2wpkh(pubkey_or_hash),
                BuilderType::p2tr_key_path(ref key_path) => self.p2tr_key_path(key_path),
                // BuilderType::p2tr_script_path(ref script) => self.p2tr_script_path(script),
                BuilderType::brc20_inscribe(ref inscription) => self.brc20_inscribe(inscription),
                BuilderType::None => SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("No Input Builder type provided"),
            },
            ScriptType::script_data(ref script) => self.custom_script(script.to_vec()),
            ScriptType::receiver_address(ref address) => self.recipient_address(address),
            ScriptType::None => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No Input claiming script provided"),
        }
    }

    // TODO next iteration
    // pub fn p2sh(&self, redeem_script: Data) -> SigningResult<(TransactionInput, UtxoToSign)> {
    //     let redeem_script = Script::from(redeem_script);
    //     self.prepare_builder()?.p2sh(redeem_script)
    // }

    pub fn p2pk(&self, pubkey: &[u8]) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let pubkey = ecdsa::secp256k1::PublicKey::try_from(pubkey)
            .into_tw()
            .context("Invalid P2PK public key")?;

        self.prepare_builder()?.p2pk(&pubkey)
    }

    pub fn p2pkh(
        &self,
        pubkey_or_hash: &Proto::PublicKeyOrHash,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let pubkey_hash = self.get_ecdsa_pubkey_from_proto(pubkey_or_hash)?;
        self.prepare_builder()?.p2pkh(&pubkey_hash)
    }

    // TODO next iteration
    // pub fn p2wsh(&self, redeem_script: Data) -> SigningResult<(TransactionInput, UtxoToSign)> {
    //     let script = Script::from(redeem_script);
    //     self.prepare_builder()?.p2wsh(script)
    // }

    pub fn p2wpkh(
        &self,
        pubkey_or_hash: &Proto::PublicKeyOrHash,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let pubkey = self.get_ecdsa_pubkey_from_proto(pubkey_or_hash)?;
        self.prepare_builder()?.p2wpkh(&pubkey)
    }

    pub fn p2tr_key_path(&self, pubkey: &[u8]) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let public_key = schnorr::PublicKey::try_from(pubkey)?;
        self.prepare_builder()?.p2tr_key_path(&public_key)
    }

    // TODO next iteration
    // pub fn p2tr_script_path(
    //     &self,
    //     taproot_script_path: &Proto::mod_Input::InputTaprootScriptPath,
    // ) -> SigningResult<(TransactionInput, UtxoToSign)> {
    //     let payload = Script::from(taproot_script_path.payload.to_vec());
    //     // let x = taproot_script_path.
    //     self.prepare_builder()?
    //         .p2tr_script_path(payload, taproot_script_path.control_block.to_vec())
    // }

    pub fn brc20_inscribe(
        &self,
        inscription: &Proto::mod_Input::InputBrc20Inscription,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let public_key = schnorr::PublicKey::try_from(inscription.inscribe_to.as_ref())?;
        self.prepare_builder()?.brc20_transfer(
            &public_key,
            inscription.ticker.to_string(),
            inscription.transfer_amount.to_string(),
        )
    }

    pub fn custom_script(
        &self,
        script_data: Data,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let script = Script::from(script_data);
        let builder = self.prepare_builder()?;

        match StandardScriptParser.parse(&script)? {
            StandardScript::P2PK(pk) => builder.p2pk(&pk),
            StandardScript::P2PKH(pubkey_hash) => {
                let pubkey = self.public_keys.get_ecdsa_public_key(&pubkey_hash)?;
                builder.p2pkh(&pubkey)
            },
            StandardScript::P2WPKH(pubkey_hash) => {
                let pubkey = self.public_keys.get_ecdsa_public_key(&pubkey_hash)?;
                builder.p2wpkh(&pubkey)
            },
            StandardScript::P2TR(tweaked_pubkey) => {
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

    pub fn recipient_address(
        &self,
        addr_str: &str,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let addr = Context::Address::from_str(addr_str)
            .into_tw()
            .context("Invalid claiming script recipient address")?;
        let claiming_script_pubkey =
            Context::addr_to_script_pubkey(&addr, self.chain_info.to_address_prefixes())?;
        self.custom_script(claiming_script_pubkey.into())
            .with_context(|| format!("Error handling {addr_str} input recipient"))
    }

    pub fn prepare_builder(&self) -> SigningResult<UtxoBuilder> {
        let OutPoint { hash, index } = parse_out_point(&self.input.out_point)?;
        let sighash_ty = SighashType::from_u32(self.input.sighash_type)?;

        if self.input.value < 0 {
            return SigningError::err(SigningErrorType::Error_invalid_utxo_amount)
                .context("UTXO amount cannot be negative");
        }

        let sequence = self
            .input
            .sequence
            .clone()
            .map(|seq| seq.sequence)
            // Use the default 0xFFFFFFFF sequence value if not specified.
            .unwrap_or(u32::MAX);

        Ok(UtxoBuilder::default()
            .prev_txid(hash)
            .prev_index(index)
            .sequence(sequence)
            .amount(self.input.value)
            .sighash_type(sighash_ty))
    }

    /// Tries to convert [`Proto::PublicKeyOrHash`] to [`Hash<N>`].
    /// Please note `P2PKH` and `P2WPKH` use the same `ripemd(sha256(x))` hash function.
    fn get_ecdsa_pubkey_from_proto(
        &self,
        input: &Proto::PublicKeyOrHash,
    ) -> SigningResult<ecdsa::secp256k1::PublicKey> {
        use Proto::mod_PublicKeyOrHash::OneOfvariant as PublicKeyOrHashType;

        let pubkey_data = match input.variant {
            PublicKeyOrHashType::pubkey(ref pubkey) => pubkey.as_ref(),
            PublicKeyOrHashType::hash(ref hash) => {
                let hash = H160::try_from(hash.as_ref())
                    .tw_err(|_| SigningErrorType::Error_invalid_params)
                    .context("Expected 20 bytes public key hash")?;
                self.public_keys.get_public_key(&hash)?
            },
            PublicKeyOrHashType::None => {
                return SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("Expected a public key or its hash")
            },
        };

        ecdsa::secp256k1::PublicKey::try_from(pubkey_data)
            .into_tw()
            .context("Expected a valid ecdsa secp256k1 public key")
    }
}

pub fn parse_out_point(maybe_out_point: &Option<Proto::OutPoint>) -> SigningResult<OutPoint> {
    let out_point = maybe_out_point
        .as_ref()
        .or_tw_err(SigningErrorType::Error_invalid_params)
        .context("No OutPoint provided for a UTXO")?;

    let hash = H256::try_from(out_point.hash.as_ref())
        .tw_err(|_| SigningErrorType::Error_invalid_params)
        .context("Invalid previous txid")?;

    Ok(OutPoint {
        hash,
        index: out_point.vout,
    })
}
