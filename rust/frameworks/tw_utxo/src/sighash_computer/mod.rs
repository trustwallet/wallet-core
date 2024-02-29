// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::{UtxoError, UtxoErrorKind, UtxoResult};
use crate::script::{Script, Witness};
use crate::sighash::SighashType;
use crate::signing_mode::SigningMethod;

use crate::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use crate::transaction::transaction_parts::Amount;
use crate::transaction::{TransactionPreimage, UtxoPreimageArgs, UtxoTaprootPreimageArgs};
use std::marker::PhantomData;
use tw_hash::hasher::Hasher;
use tw_hash::H256;
use tw_keypair::tw;
use tw_memory::Data;

const DEFAULT_TX_HASHER: Hasher = Hasher::Sha256d;

/// UTXO (Unsigned transaction input) contains all info required to sign the.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct UtxoToSign {
    pub script_pubkey: Script,
    pub signing_method: SigningMethod,
    // TODO: Rename to value?
    pub amount: Amount,
    pub leaf_hash_code_separator: Option<(H256, u32)>,
    pub tx_hasher: Hasher,
    pub sighash_ty: SighashType,
}

impl Default for UtxoToSign {
    fn default() -> Self {
        UtxoToSign {
            script_pubkey: Script::default(),
            signing_method: SigningMethod::Legacy,
            amount: Amount::default(),
            leaf_hash_code_separator: None,
            tx_hasher: DEFAULT_TX_HASHER,
            sighash_ty: SighashType::default(),
        }
    }
}

/// Transaction preimage arguments.
/// It provides required options to sign each UTXO of the transaction.
// TODO: Move this to another module.
pub struct TxSigningArgs {
    pub utxos_to_sign: Vec<UtxoToSign>,
}

impl Default for TxSigningArgs {
    fn default() -> Self {
        TxSigningArgs {
            utxos_to_sign: Vec::default(),
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxPreimage {
    /// Transaction signatures in the same order as the transaction UTXOs.
    pub sighashes: Vec<UtxoSighash>,
}

impl TxPreimage {
    /// Converts all the sighashes into a list of [`H256`] types, ready to be
    /// signed.
    pub fn into_h256_list(self) -> UtxoResult<Vec<H256>> {
        self.sighashes
            .into_iter()
            .map(|s| {
                H256::try_from(s.sighash.as_slice())
                    .map_err(|_| UtxoError(UtxoErrorKind::Error_internal))
            })
            .collect::<UtxoResult<_>>()
    }
}

#[derive(Debug, Clone)]
pub struct UtxoSighash {
    /// The signing method needs to be used for this sighash.
    pub signing_method: SigningMethod,
    pub sighash: Data,
}

// TODO: Move this to another module.
pub struct SpendingData {
    pub script_sig: Script,
    pub witness: Witness,
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
pub struct SighashComputer<Transaction> {
    transaction_to_sign: Transaction,
    args: TxSigningArgs,
    _phantom: PhantomData<Transaction>,
}

impl<Transaction> SighashComputer<Transaction>
where
    Transaction: TransactionPreimage + TransactionInterface,
{
    pub fn new(transaction: Transaction, args: TxSigningArgs) -> Self {
        SighashComputer {
            transaction_to_sign: transaction,
            args,
            _phantom: PhantomData,
        }
    }

    /// Computes sighashes of [`SighashComputer::transaction`].
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
                    leaf_hash_code_separator: utxo.leaf_hash_code_separator,
                    sighash_ty: utxo.sighash_ty,
                    tx_hasher: utxo.tx_hasher,
                    signing_method,
                };

                let sighash = match signing_method {
                    SigningMethod::Legacy | SigningMethod::Segwit => {
                        self.transaction_to_sign.preimage_tx(&utxo_args)?
                    },
                    SigningMethod::TaprootAll | SigningMethod::TaprootOnePrevout => {
                        let tr_spent_amounts: Vec<Amount> = self
                            .args
                            .utxos_to_sign
                            .iter()
                            .map(|utxo| utxo.amount)
                            .collect();

                        let tr_spent_script_pubkeys: Vec<Script> = self
                            .args
                            .utxos_to_sign
                            .iter()
                            .map(|utxo| utxo.script_pubkey.clone())
                            .collect();

                        let tr = UtxoTaprootPreimageArgs {
                            args: utxo_args,
                            spent_amounts: tr_spent_amounts,
                            spent_script_pubkeys: tr_spent_script_pubkeys.clone(),
                        };

                        self.transaction_to_sign.preimage_taproot_tx(&tr)?
                    },
                };

                Ok(UtxoSighash {
                    signing_method,
                    sighash,
                })
            })
            // Collect the results as [`UtxoResult<Vec<UtxoSighash>>`].
            .collect::<UtxoResult<Vec<_>>>()
            .map(|sighashes: Vec<UtxoSighash>| TxPreimage { sighashes })
    }

    // TODO: This comment should be rephrased, given that in validates the preimages, not the claims
    /// Validates if the given signatures and public keys correspond to the actual [`SighashComputer::transaction`] inputs.
    /// The method should be used to validate externally passed signatures before [`SighashComputer::compile`].
    ///
    /// # Hint
    ///
    /// Not required when [`SighashComputer::compile`] is called right after [`SighashComputer::preimage_tx`],
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

    /// Compiles the transaction with the given spending data.
    ///
    /// # Hint
    ///
    /// Consider using [`SighashComputer::verify_signatures`] before calling [`SighashComputer::compile`]
    /// if the signatures were computed externally.
    pub fn compile(mut self, claims: Vec<SpendingData>) -> UtxoResult<Transaction> {
        // There should be the same number of UTXOs and their meta data.
        if self.args.utxos_to_sign.len() != self.transaction_to_sign.inputs().len() {
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }
        if self.args.utxos_to_sign.len() != claims.len() {
            // TODO: Set appropriate error variant
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }

        // Add the claiming script (scriptSig or Witness) to the transaction inputs.
        for (utxo, claim) in self
            .transaction_to_sign
            .inputs_mut()
            .iter_mut()
            .zip(claims.into_iter())
        {
            // TODO: Check that at least one claim should be present.
            utxo.set_script_sig(claim.script_sig);
            utxo.set_witness(claim.witness);
        }

        Ok(self.transaction_to_sign)
    }
}
