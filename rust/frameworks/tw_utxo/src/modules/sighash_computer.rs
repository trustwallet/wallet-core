// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::script::Script;
use crate::signing_mode::SigningMethod;
use crate::transaction::transaction_interface::TransactionInterface;
use crate::transaction::transaction_parts::Amount;
use crate::transaction::unsigned_transaction::UnsignedTransaction;
use crate::transaction::{
    TransactionPreimage, UtxoPreimageArgs, UtxoTaprootPreimageArgs, UtxoToSign,
};
use std::marker::PhantomData;
use tw_coin_entry::coin_entry::PublicKeyBytes;
use tw_coin_entry::error::prelude::SigningResult;
use tw_hash::H256;

#[derive(Debug, Clone)]
pub struct TxPreimage {
    /// Transaction signatures in the same order as the transaction UTXOs.
    pub sighashes: Vec<UtxoSighash>,
}

#[derive(Debug, Clone)]
pub struct UtxoSighash {
    /// The signing method needs to be used for this sighash.
    pub signing_method: SigningMethod,
    pub sighash: H256,
    pub signer_pubkey: PublicKeyBytes,
    /// Taproot tweak if [`SigningMethod::Taproot`] signing method is used.
    /// Empty if there is no need to tweak the private to sign the sighash.
    pub taproot_tweak: Option<TaprootTweak>,
}

#[derive(Debug, Clone)]
pub struct TaprootTweak {
    /// 32 bytes merkle root of the script tree.
    /// `None` if there are no scripts, and the private key should be tweaked without a merkle root.
    pub merkle_root: Option<H256>,
}

/// Sighash Computer with a standard Bitcoin behaviour.
///
/// # Important
///
/// If needed to implement a custom logic, consider adding a different Sighash Computer.
pub struct SighashComputer<Transaction> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction> SighashComputer<Transaction>
where
    Transaction: TransactionPreimage + TransactionInterface,
{
    /// Computes sighashes of [`SighashComputer::transaction`].
    pub fn preimage_tx(
        unsigned_tx: &UnsignedTransaction<Transaction>,
    ) -> SigningResult<TxPreimage> {
        unsigned_tx
            .input_args()
            .iter()
            .enumerate()
            .map(|(input_index, utxo)| {
                let signing_method = utxo.signing_method;

                let utxo_args = UtxoPreimageArgs {
                    input_index,
                    script_pubkey: utxo.script_pubkey.clone(),
                    amount: utxo.amount,
                    // TODO move `leaf_hash_code_separator` to `UtxoTaprootPreimageArgs`.
                    leaf_hash_code_separator: utxo.leaf_hash_code_separator,
                    sighash_ty: utxo.sighash_ty,
                    tx_hasher: utxo.tx_hasher,
                    signing_method,
                };

                let (sighash, taproot_tweak) = match signing_method {
                    SigningMethod::Legacy | SigningMethod::Segwit => {
                        let sighash = unsigned_tx.transaction().preimage_tx(&utxo_args)?;
                        (sighash, None)
                    },
                    SigningMethod::Taproot => {
                        // TODO Move `tr_spent_amounts` and `tr_spent_script_pubkeys` logic to `Transaction::preimage_taproot_tx()`.
                        let tr_spent_amounts: Vec<Amount> = unsigned_tx
                            .input_args()
                            .iter()
                            .map(|utxo| utxo.amount)
                            .collect();

                        let tr_spent_script_pubkeys: Vec<Script> = unsigned_tx
                            .input_args()
                            .iter()
                            .map(|utxo| {
                                if utxo.signing_method == SigningMethod::Taproot {
                                    // Taproot UTXOs scriptPubkeys should be signed as is.
                                    utxo.script_pubkey.clone()
                                } else {
                                    // Use the original scriptPubkey declared in the unspent output.
                                    utxo.prevout_script_pubkey.clone()
                                }
                            })
                            .collect();

                        let tr = UtxoTaprootPreimageArgs {
                            args: utxo_args,
                            spent_amounts: tr_spent_amounts,
                            spent_script_pubkeys: tr_spent_script_pubkeys.clone(),
                        };

                        let sighash = unsigned_tx.transaction().preimage_taproot_tx(&tr)?;
                        let taproot_tweak = Self::get_taproot_tweak(utxo);

                        (sighash, taproot_tweak)
                    },
                };

                Ok(UtxoSighash {
                    signing_method,
                    sighash,
                    signer_pubkey: utxo.spender_public_key.clone(),
                    taproot_tweak,
                })
            })
            // Collect the results as [`SigningResult<Vec<UtxoSighash>>`].
            .collect::<SigningResult<Vec<_>>>()
            .map(|sighashes: Vec<UtxoSighash>| TxPreimage { sighashes })
    }

    pub fn get_taproot_tweak(utxo: &UtxoToSign) -> Option<TaprootTweak> {
        // Any empty leaf hash implies P2TR key-path (balance transfer)
        if utxo.leaf_hash_code_separator.is_none() {
            // Tweak keypair for P2TR key-path (ie. zeroed Merkle root).
            let merkle_root = None;
            Some(TaprootTweak { merkle_root })
        } else {
            None
        }
    }
}
