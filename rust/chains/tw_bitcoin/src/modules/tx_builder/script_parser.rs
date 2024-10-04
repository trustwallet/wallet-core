// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::tx_builder::BitcoinChainInfo;
use tw_coin_entry::error::prelude::*;
use tw_hash::{H160, H256};
use tw_keypair::{ecdsa, schnorr};
use tw_memory::Data;
use tw_utxo::address::legacy::LegacyAddress;
use tw_utxo::address::segwit::SegwitAddress;
use tw_utxo::address::standard_bitcoin::StandardBitcoinAddress;
use tw_utxo::address::taproot::TaprootAddress;
use tw_utxo::script::standard_script::conditions;
use tw_utxo::script::Script;

pub enum StandardScript {
    /// Compressed or uncompressed public key bytes.
    P2PK(ecdsa::secp256k1::PublicKey),
    /// Public key hash.
    P2PKH(H160),
    /// Script hash.
    P2SH(H160),
    /// Public key hash.
    P2WPKH(H160),
    /// Script hash.
    P2WSH(H256),
    /// Tweaked public key.
    /// The public key can be tweaked as either key-path or script-path,
    P2TR(schnorr::XOnlyPublicKey),
    /// OP_RETURN payload.
    OpReturn(Data),
}

impl StandardScript {
    pub fn try_to_address(
        &self,
        chain_info: &BitcoinChainInfo,
    ) -> AddressResult<Option<StandardBitcoinAddress>> {
        let try_hrp = || chain_info.hrp.clone().ok_or(AddressError::MissingPrefix);

        match self {
            StandardScript::P2PK(pubkey) => {
                // Display P2PK input as P2PKH.
                LegacyAddress::p2pkh_with_public_key(chain_info.p2pkh_prefix, pubkey)
                    .map(StandardBitcoinAddress::Legacy)
                    .map(Some)
            },
            StandardScript::P2PKH(pubkey_hash) => {
                LegacyAddress::new(chain_info.p2pkh_prefix, pubkey_hash.as_slice())
                    .map(StandardBitcoinAddress::Legacy)
                    .map(Some)
            },
            StandardScript::P2SH(script_hash) => {
                LegacyAddress::new(chain_info.p2sh_prefix, script_hash.as_slice())
                    .map(StandardBitcoinAddress::Legacy)
                    .map(Some)
            },
            StandardScript::P2WPKH(pubkey_hash) => {
                SegwitAddress::new(try_hrp()?, pubkey_hash.to_vec())
                    .map(StandardBitcoinAddress::Segwit)
                    .map(Some)
            },
            StandardScript::P2WSH(script_hash) => {
                SegwitAddress::new(try_hrp()?, script_hash.to_vec())
                    .map(StandardBitcoinAddress::Segwit)
                    .map(Some)
            },
            StandardScript::P2TR(tweaked_pubkey) => {
                TaprootAddress::new(try_hrp()?, tweaked_pubkey.bytes().to_vec())
                    .map(StandardBitcoinAddress::Taproot)
                    .map(Some)
            },
            StandardScript::OpReturn(_) => Ok(None),
        }
    }
}

pub struct StandardScriptParser;

impl StandardScriptParser {
    /// Later, this method can be moved to a trait.
    pub fn parse(&self, script: &Script) -> SigningResult<StandardScript> {
        if let Some(pubkey) = conditions::match_p2pk(script) {
            // P2PK
            let pubkey = ecdsa::secp256k1::PublicKey::try_from(pubkey)
                .into_tw()
                .context("P2PK scriptPubkey must contain a valid ecdsa secp256k1 public key")?;
            Ok(StandardScript::P2PK(pubkey))
        } else if let Some(pubkey_hash) = conditions::match_p2pkh(script) {
            // P2PKH
            Ok(StandardScript::P2PKH(pubkey_hash))
        } else if let Some(script_hash) = conditions::match_p2sh(script) {
            // P2SH
            Ok(StandardScript::P2SH(script_hash))
        } else if let Some(pubkey_hash) = conditions::match_p2wpkh(script) {
            // P2WPKH
            Ok(StandardScript::P2WPKH(pubkey_hash))
        } else if let Some(script_hash) = conditions::match_p2wsh(script) {
            // P2WSH
            Ok(StandardScript::P2WSH(script_hash))
        } else if let Some(tweaked_pubkey) = conditions::match_p2tr(script) {
            // P2TR
            let tweaked_pubkey_x_only =
                schnorr::XOnlyPublicKey::try_from(tweaked_pubkey.as_slice())
                    .into_tw()
                    .context("P2TR scriptPubkey must contain a valid tweaked schnorr public key")?;
            Ok(StandardScript::P2TR(tweaked_pubkey_x_only))
        } else if let Some(payload) = conditions::match_op_return(script) {
            // OP_RETURN
            Ok(StandardScript::OpReturn(payload))
        } else {
            // Unknown
            SigningError::err(SigningErrorType::Error_script_output).context(
                "The given custom scriptPubkey is not supported. Consider using a proper Input/Output builder",
            )
        }
    }
}
