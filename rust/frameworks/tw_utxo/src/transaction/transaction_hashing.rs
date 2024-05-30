// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;

use crate::sighash::SighashBase;
use crate::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use crate::transaction::UtxoPreimageArgs;
use std::marker::PhantomData;
use tw_hash::hasher::Hasher;
use tw_memory::Data;

use super::UtxoTaprootPreimageArgs;

/// A helper structure that hashes some parts of the transaction.
pub struct TransactionHasher<Transaction> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction: TransactionInterface> TransactionHasher<Transaction> {
    /// Computes a hash of all [`SignedUtxo::previour_output`].
    pub fn prevout_hash(tx: &Transaction, tx_hasher: Hasher) -> Data {
        let mut stream = Stream::default();
        for input in tx.inputs() {
            stream.append(input.previous_output());
        }
        tx_hasher.hash(&stream.out())
    }

    /// Returns a zero hash if [`UtxoPreimageArgs::sighash::anyone_can_pay`] is true,
    /// otherwise returns a hash of all [`TransactionInput::previour_output`].
    pub fn preimage_prevout_hash(tx: &Transaction, args: &UtxoPreimageArgs) -> Data {
        if args.sighash_ty.anyone_can_pay() {
            return args.tx_hasher.zero_hash();
        }
        Self::prevout_hash(tx, args.tx_hasher)
    }

    /// Computes a hash of all `spent_amounts`. Required for TapSighash.
    pub fn spent_amount_hash(tr: &UtxoTaprootPreimageArgs) -> Data {
        let mut stream = Stream::default();
        for amount in &tr.spent_amounts {
            stream.append(amount);
        }
        tr.args.tx_hasher.hash(&stream.out())
    }

    /// Computes a hash of all `script_pubkeys`. Required for TapSighash.
    pub fn spent_script_pubkeys(tr: &UtxoTaprootPreimageArgs) -> Data {
        let mut stream = Stream::default();
        for script in &tr.spent_script_pubkeys {
            stream.append(script);
        }
        tr.args.tx_hasher.hash(&stream.out())
    }

    /// Computes a hash of all [`SignedUtxo::sequence`].
    pub fn sequence_hash(tx: &Transaction, tx_hasher: Hasher) -> Data {
        let mut stream = Stream::default();
        for input in tx.inputs() {
            stream.append(&input.sequence());
        }
        tx_hasher.hash(&stream.out())
    }

    /// Returns a zero hash if [`UtxoPreimageArgs::sighash`] requires it (see the code below),
    /// otherwise returns a hash of all [`TransactionInput::sequence`].
    pub fn preimage_sequence_hash(tx: &Transaction, args: &UtxoPreimageArgs) -> Data {
        let single_or_none = matches!(
            args.sighash_ty.base_type(),
            SighashBase::Single | SighashBase::None
        );
        if args.sighash_ty.anyone_can_pay() || single_or_none {
            return args.tx_hasher.zero_hash();
        }
        Self::sequence_hash(tx, args.tx_hasher)
    }

    /// Returns a hash of required [`TransactionOutput`] according to the [`UtxoPreimageArgs::sighash`].
    /// Please note the function can return a zero hash if necessary.
    pub fn preimage_outputs_hash(tx: &Transaction, args: &UtxoPreimageArgs) -> Data {
        let outputs = tx.outputs();
        match args.sighash_ty.base_type() {
            SighashBase::All => {
                let mut stream = Stream::default();
                for output in outputs {
                    stream.append(output);
                }
                args.tx_hasher.hash(&stream.out())
            },
            SighashBase::Single if args.input_index < outputs.len() => {
                let mut stream = Stream::default();
                stream.append(&outputs[args.input_index]);
                args.tx_hasher.hash(&stream.out())
            },
            _ => args.tx_hasher.zero_hash(),
        }
    }
}
