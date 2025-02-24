// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;

use crate::sighash::{SighashBase, SighashType};
use crate::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use std::marker::PhantomData;
use tw_hash::hasher::StatefulHasher;
use tw_memory::Data;

use super::UtxoPreimageArgs;

/// A helper structure that hashes some parts of the transaction.
pub struct TransactionHasher<Transaction> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction: TransactionInterface> TransactionHasher<Transaction> {
    /// Computes a hash of all [`SignedUtxo::previour_output`].
    pub fn prevout_hash<Hasher: StatefulHasher>(tx: &Transaction, tx_hasher: Hasher) -> Data {
        let mut stream = Stream::default();
        for input in tx.inputs() {
            stream.append(input.previous_output());
        }
        tx_hasher.hash(&stream.out())
    }

    /// Returns a zero hash if [`UtxoPreimageArgs::sighash::anyone_can_pay`] is true,
    /// otherwise returns a hash of all [`TransactionInput::previour_output`].
    pub fn preimage_prevout_hash<Hasher: StatefulHasher>(
        tx: &Transaction,
        sighash_ty: SighashType,
        tx_hasher: Hasher,
    ) -> Data {
        if sighash_ty.anyone_can_pay() {
            return tx_hasher.zero_hash();
        }
        Self::prevout_hash(tx, tx_hasher)
    }

    /// Computes a hash of all `spent_amounts`. Required for TapSighash.
    pub fn spent_amount_hash(args: &UtxoPreimageArgs) -> Data {
        let mut stream = Stream::default();
        for amount in &args.taproot_args.spent_amounts {
            stream.append(amount);
        }
        args.tx_hasher.hash(&stream.out())
    }

    /// Computes a hash of all `script_pubkeys`. Required for TapSighash.
    pub fn spent_script_pubkeys(args: &UtxoPreimageArgs) -> Data {
        let mut stream = Stream::default();
        for script in &args.taproot_args.spent_script_pubkeys {
            stream.append(script);
        }
        args.tx_hasher.hash(&stream.out())
    }

    /// Computes a hash of all [`SignedUtxo::sequence`].
    pub fn sequence_hash<Hasher: StatefulHasher>(tx: &Transaction, tx_hasher: Hasher) -> Data {
        let mut stream = Stream::default();
        for input in tx.inputs() {
            stream.append(&input.sequence());
        }
        tx_hasher.hash(&stream.out())
    }

    /// Returns a zero hash if [`UtxoPreimageArgs::sighash`] requires it (see the code below),
    /// otherwise returns a hash of all [`TransactionInput::sequence`].
    pub fn preimage_sequence_hash<Hasher: StatefulHasher>(
        tx: &Transaction,
        sighash_ty: SighashType,
        tx_hasher: Hasher,
    ) -> Data {
        let single_or_none = matches!(
            sighash_ty.base_type(),
            SighashBase::Single | SighashBase::None
        );
        if sighash_ty.anyone_can_pay() || single_or_none {
            return tx_hasher.zero_hash();
        }
        Self::sequence_hash(tx, tx_hasher)
    }

    /// Returns a hash of required [`TransactionOutput`] according to the [`UtxoPreimageArgs::sighash`].
    /// Please note the function can return a zero hash if necessary.
    pub fn preimage_outputs_hash<Hasher: StatefulHasher>(
        tx: &Transaction,
        input_index: usize,
        sighash_ty: SighashType,
        tx_hasher: Hasher,
    ) -> Data {
        let outputs = tx.outputs();
        match sighash_ty.base_type() {
            SighashBase::All => {
                let mut stream = Stream::default();
                for output in outputs {
                    stream.append(output);
                }
                tx_hasher.hash(&stream.out())
            },
            SighashBase::Single if input_index < outputs.len() => {
                let mut stream = Stream::default();
                stream.append(&outputs[input_index]);
                tx_hasher.hash(&stream.out())
            },
            _ => tx_hasher.zero_hash(),
        }
    }
}
