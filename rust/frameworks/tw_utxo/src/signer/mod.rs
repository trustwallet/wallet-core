// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::{UtxoError, UtxoErrorKind, UtxoResult};
use crate::script::Script;
use crate::sighash::Sighash;
use crate::signing_mode::SigningMethod;
use crate::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use crate::transaction::transaction_parts::Amount;
use crate::transaction::{TransactionPreimage, UtxoPreimageArgs};
use std::marker::PhantomData;
use tw_hash::hasher::Hasher;
use tw_keypair::tw;
use tw_memory::Data;

const DEFAULT_TX_HASHER: Hasher = Hasher::Sha256d;

/// UTXO (Unsigned transaction input) contains all info required to sign the.
pub struct UtxoToSign {
    script_pubkey: Script,
    signing_method: SigningMethod,
    amount: Amount,
}

/// Transaction preimage arguments.
/// It provides required options to sign each UTXO of the transaction.
pub struct TxSigningArgs {
    pub utxos_to_sign: Vec<UtxoToSign>,
    pub sighash: Sighash,
    pub tx_hasher: Hasher,
}

impl Default for TxSigningArgs {
    fn default() -> Self {
        TxSigningArgs {
            utxos_to_sign: Vec::default(),
            sighash: Sighash::default(),
            tx_hasher: DEFAULT_TX_HASHER,
        }
    }
}

pub struct TxPreimage {
    /// Transaction signatures in the same order as the transaction UTXOs.
    pub sighashes: Vec<UtxoSighash>,
}

pub struct UtxoSighash {
    /// The signing method needs to be used for this sighash.
    pub signing_method: SigningMethod,
    pub sighash: Data,
}

pub struct ClaimingData {
    pub script_sig: Script,
    pub witness: Vec<Script>,
}

/// A pair of a signature and corresponding public key.
pub struct SignaturePubkey {
    pub signature: tw::Signature,
    pub public_key: tw::PublicKey,
}

/// Transaction signer with a standard signing behaviour.
///
/// # Important
///
/// If needed to implement a custom logic, consider adding a different signer.
pub struct TransactionSigner<Transaction> {
    transaction_to_sign: Transaction,
    args: TxSigningArgs,
    _phantom: PhantomData<Transaction>,
}

impl<Transaction> TransactionSigner<Transaction>
where
    Transaction: TransactionPreimage + TransactionInterface,
{
    pub fn new(transaction: Transaction) -> Self {
        TransactionSigner {
            transaction_to_sign: transaction,
            args: TxSigningArgs::default(),
            _phantom: PhantomData,
        }
    }

    pub fn set_signing_args(&mut self, args: TxSigningArgs) {
        self.args = args;
    }

    /// Computes sighashes of [`TransactionSigner::transaction`].
    pub fn preimage_tx(&self) -> UtxoResult<TxPreimage> {
        // There should be the same number of UTXOs and their meta data.
        if self.args.utxos_to_sign.len() != self.transaction_to_sign.inputs().len() {
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }

        self.args
            .utxos_to_sign
            .iter()
            .enumerate()
            .map(|(input_index, utxo)| {
                let signing_method = utxo.signing_method;

                let utxo_args = UtxoPreimageArgs {
                    input_index,
                    script_pubkey: utxo.script_pubkey.clone(),
                    amount: utxo.amount,
                    sighash: self.args.sighash,
                    tx_hasher: self.args.tx_hasher,
                    signing_method,
                };

                let sighash = self.transaction_to_sign.preimage_tx(&utxo_args)?;

                Ok(UtxoSighash {
                    signing_method,
                    sighash,
                })
            })
            // Collect the results as [`UtxoResult<Vec<UtxoSighash>>`].
            .collect::<UtxoResult<Vec<_>>>()
            .map(|sighashes: Vec<UtxoSighash>| TxPreimage { sighashes })
    }

    /// Validates if the given signatures and public keys correspond to the actual [`TransactionSigner::transaction`] inputs.
    /// The method should be used to validate externally passed signatures before [`TransactionSigner::compile`].
    ///
    /// # Hint
    ///
    /// Not required when [`TransactionSigner::compile`] is called right after [`TransactionSigner::preimage_tx`],
    /// as this method is expensive in terms of computations.
    pub fn verify_signatures(&self, signatures: Vec<SignaturePubkey>) -> UtxoResult<bool> {
        // Compute transaction preimage and verify if all given signatures correspond to the result sighashes.
        let tx_preimage = self.preimage_tx()?;

        if tx_preimage.sighashes.len() != signatures.len() {
            return Err(UtxoError(UtxoErrorKind::Error_signatures_count));
        }

        let all_valid =
            tx_preimage
                .sighashes
                .iter()
                .zip(signatures.iter())
                .all(|(sighash, sig_pubkey)| {
                    sig_pubkey
                        .public_key
                        .verify(&sig_pubkey.signature, &sighash.sighash)
                });

        Ok(all_valid)
    }

    /// Compiles a transaction with the given signatures.
    /// The signatures must be in the same order as corresponding UTXOs.
    ///
    /// # Hint
    ///
    /// Consider using [`TransactionSigner::verify_signatures`] before calling [`TransactionSigner::compile`]
    /// if the signatures were computed externally.
    pub fn compile(mut self, claims: Vec<ClaimingData>) -> UtxoResult<Transaction> {
        // There should be the same number of UTXOs and their meta data.
        if self.args.utxos_to_sign.len() != self.transaction_to_sign.inputs().len() {
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }
        if self.args.utxos_to_sign.len() != claims.len() {
            // TODO: Set appropriate error variant
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }

        for (utxo, claim) in self
            .transaction_to_sign
            .inputs_mut()
            .iter_mut()
            .zip(claims.into_iter())
        {
            // TODO: At least one claim should be present.
            utxo.set_script_sig(claim.script_sig);
            utxo.set_witness(claim.witness);
        }

        todo!()
    }
}
