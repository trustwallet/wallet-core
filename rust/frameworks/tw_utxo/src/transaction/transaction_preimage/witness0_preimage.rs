// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::error::{UtxoError, UtxoErrorKind, UtxoResult};
use crate::transaction::transaction_hashing::TransactionHasher;
use crate::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use crate::transaction::PreimageArgs;
use std::marker::PhantomData;
use tw_memory::Data;

/// `LegacyPreimage` should be used if only a transaction does not have segwit inputs (with segwit).
pub struct Witness0Preimage<Transaction: TransactionInterface> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction: TransactionInterface> Witness0Preimage<Transaction> {
    pub fn sighash_tx(tx: &Transaction, args: &PreimageArgs) -> UtxoResult<Data> {
        let input = tx
            .inputs()
            .get(args.input_index)
            .ok_or(UtxoError(UtxoErrorKind::Error_sighash_failed))?;

        let prevout_hash = TransactionHasher::<Transaction>::preimage_prevout_hash(tx, args);
        let sequence_hash = TransactionHasher::<Transaction>::preimage_sequence_hash(tx, args);
        let outputs_hash = TransactionHasher::<Transaction>::preimage_outputs_hash(tx, args);

        let mut stream = Stream::default();

        stream
            .append(&tx.version())
            .append_raw_slice(&prevout_hash)
            .append_raw_slice(&sequence_hash);

        // The input being signed (replacing the scriptSig with scriptCode + amount)
        // The prevout may already be contained in hashPrevout, and the nSequence
        // may already be contained in hashSequence.
        stream
            .append(input.previous_output())
            .append(&args.input_claiming_script)
            .append(&args.input_amount)
            .append(&input.sequence())
            .append_raw_slice(&outputs_hash)
            .append(&tx.locktime())
            // This also includes 24-bit fork id. For example, 0 for BitcoinCash.
            .append(&args.sighash.raw_sighash());

        Ok(args.tx_hasher.hash(&stream.out()))
    }
}
