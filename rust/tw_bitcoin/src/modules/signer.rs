// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::planner::BitcoinPlanner;
use crate::modules::protobuf_builder::ProtobufBuilder;
use crate::modules::signing_request::SigningRequest;
use std::borrow::Cow;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::{ecdsa, schnorr};
use tw_proto::BitcoinV3::Proto;
use tw_utxo::modules::keys_manager::KeysManager;
use tw_utxo::modules::tx_signer::TxSigner;
use tw_utxo::modules::utxo_selector::SelectResult;
use tw_utxo::signing_mode::SigningMethod;
use tw_utxo::transaction::transaction_fee::TransactionFee;

pub struct BitcoinSigner;

impl BitcoinSigner {
    pub fn sign(input: &Proto::SigningInput<'_>) -> Proto::SigningOutput<'static> {
        Self::sign_impl(input).unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    pub fn sign_impl(
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let request = SigningRequest::from_proto(input)?;
        let fee_per_vbyte = request.fee_per_vbyte;
        let SelectResult { unsigned_tx, .. } = BitcoinPlanner::plan_tx(request)?;

        let has_taproot = unsigned_tx
            .input_args()
            .iter()
            .any(|utxo_args| utxo_args.signing_method == SigningMethod::Taproot);

        let mut keys_manager = KeysManager::default();

        // Parse private keys and put them to the keys manager.
        for private in input.private_keys.iter() {
            let ecdsa_private = ecdsa::secp256k1::PrivateKey::try_from(private.as_ref())
                .into_tw()
                .context("Invalid ecdsa secp256k1 private key")?;
            keys_manager.add_ecdsa_private(ecdsa_private);

            if has_taproot {
                let schnorr_private = schnorr::PrivateKey::try_from(private.as_ref())
                    .into_tw()
                    .context("Invalid schnorr private key")?;
                keys_manager.add_schnorr_private(schnorr_private);
            }
        }

        let signed_tx =
            TxSigner::sign_tx(unsigned_tx, &keys_manager).context("Error signing transaction")?;

        let tx_encoded = signed_tx.encode_out();
        let txid = signed_tx.txid();
        let vsize = signed_tx.vsize() as u64;
        let fee = signed_tx.fee(fee_per_vbyte)? as u64;

        Ok(Proto::SigningOutput {
            transaction: Some(ProtobufBuilder::tx_to_proto(&signed_tx)),
            encoded: Cow::from(tx_encoded),
            txid: Cow::from(txid),
            vsize,
            fee,
            ..Proto::SigningOutput::default()
        })
    }
}
