// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::keys_manager::KeysManager;
use crate::modules::sighash_computer::{SighashComputer, TxPreimage, UtxoSighash};
use crate::modules::tx_compiler::TxCompiler;
use crate::signing_mode::SigningMethod;
use crate::transaction::transaction_interface::TransactionInterface;
use crate::transaction::unsigned_transaction::UnsignedTransaction;
use crate::transaction::TransactionPreimage;
use std::marker::PhantomData;
use tw_coin_entry::coin_entry::SignatureBytes;
use tw_coin_entry::error::prelude::*;
use tw_keypair::traits::SigningKeyTrait;
use tw_keypair::{ecdsa, schnorr};
use tw_misc::traits::ToBytesVec;

/// Transaction Signer with a standard Bitcoin behaviour.
///
/// # Important
///
/// If needed to implement a custom logic, consider adding a different Transaction Signer.
pub struct TxSigner<Transaction> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction> TxSigner<Transaction>
where
    Transaction: TransactionPreimage + TransactionInterface,
{
    pub fn sign_tx(
        unsigned_tx: UnsignedTransaction<Transaction>,
        keys_manager: &KeysManager,
    ) -> SigningResult<Transaction> {
        let TxPreimage { sighashes } =
            SighashComputer::preimage_tx(&unsigned_tx).context("Error sighash pre-imaging")?;

        let signatures: Vec<_> = sighashes
            .iter()
            .map(|sighash| match sighash.signing_method {
                SigningMethod::Legacy | SigningMethod::Segwit => {
                    Self::sign_legacy_sighash(keys_manager, sighash)
                },
                SigningMethod::Taproot => Self::sign_taproot_sighash(keys_manager, sighash),
            })
            .collect::<SigningResult<_>>()?;

        TxCompiler::compile(unsigned_tx, &signatures)
    }

    pub fn sign_legacy_sighash(
        keys_manager: &KeysManager,
        sighash: &UtxoSighash,
    ) -> SigningResult<SignatureBytes> {
        let public_key = ecdsa::secp256k1::PublicKey::try_from(sighash.signer_pubkey.as_slice())
            .into_tw()
            .context("Signing error: expected a valid signer ecdsa secp256k1 public key")?;

        let private_key = keys_manager.get_ecdsa_private(&public_key)?;
        let signature = private_key
            .sign(sighash.sighash)
            .into_tw()
            .context("Error signing sighash with a secp256k1 private key")?;

        Ok(signature.to_vec())
    }

    pub fn sign_taproot_sighash(
        keys_manager: &KeysManager,
        sighash: &UtxoSighash,
    ) -> SigningResult<SignatureBytes> {
        let x_only_pubkey = schnorr::XOnlyPublicKey::try_from(sighash.signer_pubkey.as_slice())
            .into_tw()
            .context("Signing error: expected a valid signer schnorr x-only public key")?;

        let private_key =
            keys_manager.get_schnorr_private(&x_only_pubkey, &sighash.taproot_tweak)?;
        let signature = private_key
            .sign(sighash.sighash)
            .into_tw()
            .context("Error signing sighash with a schnorr private key")?;

        Ok(signature.to_vec())
    }
}
