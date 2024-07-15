// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::transaction::transaction_hashing::TransactionHasher;
use crate::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use crate::transaction::UtxoPreimageArgs;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_hash::H256;

/// `Witness0Sighash`is used to calculate a preimage hash of a P2WPKH or P2WSH unspent output.
pub struct Witness0Sighash<Transaction: TransactionInterface> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction: TransactionInterface> Witness0Sighash<Transaction> {
    pub fn sighash_tx(tx: &Transaction, args: &UtxoPreimageArgs) -> SigningResult<H256> {
        let input = tx
            .inputs()
            .get(args.input_index)
            .or_tw_err(SigningErrorType::Error_internal)
            .context("Witness sighash error: input_index is out of bounds")?;

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
            .append(&args.script_pubkey)
            .append(&args.amount)
            .append(&input.sequence())
            .append_raw_slice(&outputs_hash)
            .append(&tx.locktime())
            // This also includes 24-bit fork id. For example, 0 for BitcoinCash.
            .append(&args.sighash_ty.raw_sighash());

        let hash = args.tx_hasher.hash(&stream.out());
        H256::try_from(hash.as_slice())
            .tw_err(|_| SigningErrorType::Error_internal)
            .context("Bitcoin sighash must be H256")
    }
}
