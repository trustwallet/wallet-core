// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_hash::H160;
use tw_keypair::{ecdsa, schnorr};
use tw_utxo::script::standard_script::conditions;
use tw_utxo::script::Script;

pub enum ConditionScript {
    /// Compressed or uncompressed public key bytes.
    P2PK(ecdsa::secp256k1::PublicKey),
    /// Public key hash.
    P2PKH(H160),
    /// Public key hash.
    P2WPKH(H160),
    /// Tweaked public key.
    /// The public key can be tweaked as either key-path or script-path,
    P2TR(schnorr::XOnlyPublicKey),
}

pub struct ConditionScriptParser;

impl ConditionScriptParser {
    /// Later, this method can be moved to a trait.
    pub fn parse(&self, script: &Script) -> SigningResult<ConditionScript> {
        if let Some(pubkey) = conditions::match_p2pk(script) {
            // P2PK
            let pubkey = ecdsa::secp256k1::PublicKey::try_from(pubkey)
                .into_tw()
                .context("P2PK scriptPubkey must contain a valid ecdsa secp256k1 public key")?;
            Ok(ConditionScript::P2PK(pubkey))
        } else if let Some(pubkey_hash) = conditions::match_p2pkh(script) {
            // P2PKH
            Ok(ConditionScript::P2PKH(pubkey_hash))
        } else if let Some(pubkey_hash) = conditions::match_p2wpkh(script) {
            // P2WPKH
            Ok(ConditionScript::P2WPKH(pubkey_hash))
        } else if let Some(tweaked_pubkey) = conditions::match_p2tr(script) {
            // P2TR
            let tweaked_pubkey_x_only =
                schnorr::XOnlyPublicKey::try_from(tweaked_pubkey.as_slice())
                    .into_tw()
                    .context("P2TR scriptPubkey must contain a valid tweaked schnorr public key")?;
            Ok(ConditionScript::P2TR(tweaked_pubkey_x_only))
        } else if conditions::is_p2sh(script) || conditions::is_p2wsh(script) {
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
}
