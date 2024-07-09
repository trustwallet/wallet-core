// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::tx_builder::public_keys::PublicKeys;
use crate::modules::tx_builder::BitcoinChainInfo;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::{H160, H256};
use tw_keypair::{ecdsa, schnorr};
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::address::legacy::LegacyAddress;
use tw_utxo::address::segwit::SegwitAddress;
use tw_utxo::address::standard_bitcoin::StandardBitcoinAddress;
use tw_utxo::address::taproot::TaprootAddress;
use tw_utxo::script::standard_script::conditions;
use tw_utxo::script::Script;
use tw_utxo::sighash::SighashType;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;
use tw_utxo::transaction::standard_transaction::TransactionInput;
use tw_utxo::transaction::transaction_parts::OutPoint;
use tw_utxo::transaction::UtxoToSign;

pub struct UtxoProtobuf<'a> {
    chain_info: &'a BitcoinChainInfo,
    input: &'a Proto::Input<'a>,
    public_keys: &'a PublicKeys,
}

impl<'a> UtxoProtobuf<'a> {
    pub fn new(
        chain_info: &'a BitcoinChainInfo,
        input: &'a Proto::Input<'a>,
        public_keys: &'a PublicKeys,
    ) -> Self {
        UtxoProtobuf {
            chain_info,
            input,
            public_keys,
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

        if let Some(pubkey) = conditions::match_p2pk(&script) {
            // P2PK
            let pubkey = ecdsa::secp256k1::PublicKey::try_from(pubkey.as_slice())
                .into_tw()
                .context("P2PK scriptPubkey must contain a valid ecdsa secp256k1 public key")?;
            builder.p2pk(&pubkey)
        } else if let Some(pubkey_hash) = conditions::match_p2pkh(&script) {
            // P2PKH
            let pubkey = self.get_ecdsa_pubkey_from_hash(&pubkey_hash)?;
            builder.p2pkh(&pubkey)
        } else if let Some(pubkey_hash) = conditions::match_p2wpkh(&script) {
            // P2WPKH
            let pubkey = self.get_ecdsa_pubkey_from_hash(&pubkey_hash)?;
            builder.p2wpkh(&pubkey)
        } else if let Some(tweaked_pubkey) = conditions::match_p2tr(&script) {
            // P2TR
            let tweaked_pubkey_x_only =
                schnorr::XOnlyPublicKey::try_from(tweaked_pubkey.as_slice())
                    .into_tw()
                    .context("P2TR scriptPubkey must contain a valid tweaked schnorr public key")?;
            builder.p2tr_key_path_with_tweaked_pubkey(&tweaked_pubkey_x_only)
        } else if conditions::is_p2sh(&script) || conditions::is_p2wsh(&script) {
            // P2SH or P2WSH
            SigningError::err(SigningErrorType::Error_script_output)
                .context("P2SH and P2WSH scriptPubkey's are not supported yet")
        } else {
            // Unknown
            SigningError::err(SigningErrorType::Error_script_output).context(
                "The given custom scriptPubkey is not supported. Consider using a Proto.Input.InputBuilder",
            )
        }
    }

    pub fn recipient_address(&self, addr: &str) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let addr = StandardBitcoinAddress::from_str(addr)
            .into_tw()
            .context("Invalid claiming script recipient address")?;

        match addr {
            StandardBitcoinAddress::Legacy(ref legacy) => self.recipient_legacy_address(legacy),
            StandardBitcoinAddress::Segwit(ref segwit) => self.recipient_segwit_address(segwit),
            StandardBitcoinAddress::Taproot(ref taproot) => self.recipient_taproot_address(taproot),
        }
        .with_context(|| format!("Error handling {addr} input recipient"))
    }

    pub fn recipient_legacy_address(
        &self,
        addr: &LegacyAddress,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let p2pkh_prefix = self.chain_info.p2pkh_prefix;
        let p2sh_prefix = self.chain_info.p2sh_prefix;

        if p2pkh_prefix == addr.prefix() {
            // P2PKH
            let pubkey = self.get_ecdsa_pubkey_from_hash(&addr.payload())?;
            self.prepare_builder()?.p2pkh(&pubkey)
        } else if p2sh_prefix == addr.prefix() {
            // P2SH
            SigningError::err(SigningErrorType::Error_script_redeem).context(
                "pay-to-script-hash can only be used via 'Input.InputBuilder.p2sh'.\
                 That is because P2SH address does not provide redeem script but its hash",
            )
        } else {
            // Unknown
            SigningError::err(SigningErrorType::Error_invalid_address).context(format!(
                "The given '{addr}' address has unexpected prefix. Expected p2pkh={p2pkh_prefix} p2sh={p2sh_prefix}",
            ))
        }
    }

    pub fn recipient_segwit_address(
        &self,
        addr: &SegwitAddress,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let witness_program = addr.witness_program();
        match witness_program.len() {
            // P2WPKH
            H160::LEN => {
                let pubkey_hash = H160::try_from(witness_program)
                    .expect("'witness_program' length must be checked already");
                let pubkey = self.get_ecdsa_pubkey_from_hash(&pubkey_hash)?;
                self.prepare_builder()?.p2wpkh(&pubkey)
            },
            // P2WSH
            H256::LEN => {
                SigningError::err(SigningErrorType::Error_script_redeem).context(
                    "pay-to-witness-script-hash can only be used via 'Input.InputBuilder.p2wsh'.\
                    That is because P2SH address does not provide redeem script but its hash"
                )
            },
            // Unknown
            _ => SigningError::err(SigningErrorType::Error_invalid_address)
                .context(format!("The given '{addr}' segwit address should have 20 or 32 byte length witness program")),
        }
    }

    pub fn recipient_taproot_address(
        &self,
        addr: &TaprootAddress,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let tweaked_pubkey = schnorr::XOnlyPublicKey::try_from(addr.witness_program())
            .tw_err(|_| SigningErrorType::Error_invalid_address)
            .with_context(|| {
                format!("The given '{addr}' taproot address witness program should be a valid tweaked schnorr public key")
            })?;

        self.prepare_builder()?
            .p2tr_key_path_with_tweaked_pubkey(&tweaked_pubkey)
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

    fn get_ecdsa_pubkey_from_hash(
        &self,
        pubkey_hash: &H160,
    ) -> SigningResult<ecdsa::secp256k1::PublicKey> {
        let pubkey_data = self.public_keys.get_public_key(pubkey_hash)?;
        ecdsa::secp256k1::PublicKey::try_from(pubkey_data)
            .into_tw()
            .context("Expected a valid ecdsa secp256k1 public key")
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
