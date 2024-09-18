// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::protobuf_builder::ProtobufBuilder;
use crate::modules::psbt::update_psbt_signed;
use crate::modules::psbt_request::PsbtRequest;
use crate::modules::signing_request::SigningRequestBuilder;
use std::borrow::Cow;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::{ecdsa, schnorr};
use tw_proto::BitcoinV2::Proto;
use tw_utxo::modules::keys_manager::KeysManager;
use tw_utxo::modules::tx_planner::TxPlanner;
use tw_utxo::modules::tx_signer::TxSigner;
use tw_utxo::modules::utxo_selector::SelectResult;
use tw_utxo::signing_mode::SigningMethod;
use tw_utxo::transaction::standard_transaction::Transaction;
use tw_utxo::transaction::transaction_interface::TransactionInterface;
use tw_utxo::transaction::unsigned_transaction::UnsignedTransaction;

pub struct BitcoinSigner;

impl BitcoinSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    pub fn sign_impl(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let request = SigningRequestBuilder::build(coin, input)?;
        let SelectResult { unsigned_tx, plan } = TxPlanner::plan(request)?;

        let keys_manager = Self::keys_manager_for_tx(
            &input.private_keys,
            &unsigned_tx,
            input.dangerous_use_fixed_schnorr_rng,
        )?;

        let signed_tx =
            TxSigner::sign_tx(unsigned_tx, &keys_manager).context("Error signing transaction")?;

        Ok(Proto::SigningOutput {
            transaction: Some(ProtobufBuilder::tx_to_proto(&signed_tx)),
            encoded: Cow::from(signed_tx.encode_out()),
            txid: Cow::from(signed_tx.txid()),
            // `vsize` could have been changed after the transaction being signed.
            vsize: signed_tx.vsize() as u64,
            // `fee` should haven't been changed since it's a difference between `sum(inputs)` and `sum(outputs)`.
            fee: plan.fee_estimate,
            weight: signed_tx.weight() as u64,
            ..Proto::SigningOutput::default()
        })
    }

    pub fn sign_psbt(
        coin: &dyn CoinContext,
        input: &Proto::PsbtSigningInput<'_>,
    ) -> Proto::PsbtSigningOutput<'static> {
        Self::sign_psbt_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::PsbtSigningOutput, e))
    }

    pub fn sign_psbt_impl(
        _coin: &dyn CoinContext,
        input: &Proto::PsbtSigningInput<'_>,
    ) -> SigningResult<Proto::PsbtSigningOutput<'static>> {
        let PsbtRequest {
            mut psbt,
            unsigned_tx,
        } = PsbtRequest::build(input)?;

        let fee = unsigned_tx
            .total_input()?
            .checked_sub(unsigned_tx.total_output()?)
            .or_tw_err(SigningErrorType::Error_not_enough_utxos)
            .context("PSBT sum(input) < sum(output)")?;

        let keys_manager = Self::keys_manager_for_tx(
            &input.private_keys,
            &unsigned_tx,
            input.dangerous_use_fixed_schnorr_rng,
        )?;

        let signed_tx =
            TxSigner::sign_tx(unsigned_tx, &keys_manager).context("Error signing transaction")?;

        update_psbt_signed(&mut psbt, &signed_tx);

        Ok(Proto::PsbtSigningOutput {
            transaction: Some(ProtobufBuilder::tx_to_proto(&signed_tx)),
            encoded: Cow::from(signed_tx.encode_out()),
            txid: Cow::from(signed_tx.txid()),
            // `vsize` could have been changed after the transaction being signed.
            vsize: signed_tx.vsize() as u64,
            fee,
            weight: signed_tx.weight() as u64,
            psbt: Cow::from(psbt.serialize()),
            ..Proto::PsbtSigningOutput::default()
        })
    }

    fn keys_manager_for_tx<P>(
        private_keys: &[P],
        unsigned_tx: &UnsignedTransaction<Transaction>,
        dangerous_use_fixed_schnorr_rng: bool,
    ) -> SigningResult<KeysManager>
    where
        P: AsRef<[u8]>,
    {
        let has_taproot = unsigned_tx
            .input_args()
            .iter()
            .any(|utxo_args| utxo_args.signing_method == SigningMethod::Taproot);

        let mut keys_manager = KeysManager::default();

        // Parse private keys and put them to the keys manager.
        for private in private_keys.iter() {
            let ecdsa_private = ecdsa::secp256k1::PrivateKey::try_from(private.as_ref())
                .into_tw()
                .context("Invalid ecdsa secp256k1 private key")?;
            keys_manager.add_ecdsa_private(ecdsa_private);

            if has_taproot {
                let schnorr_private = schnorr::PrivateKey::try_from(private.as_ref())
                    .into_tw()
                    .context("Invalid schnorr private key")?;

                if dangerous_use_fixed_schnorr_rng {
                    keys_manager.add_schnorr_private(schnorr_private.no_aux_rand());
                } else {
                    keys_manager.add_schnorr_private(schnorr_private);
                }
            }
        }

        Ok(keys_manager)
    }
}
