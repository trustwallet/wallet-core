// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::error::{UtxoError, UtxoErrorKind, UtxoResult};
use crate::sighash::{SighashBase, SighashType};
use crate::transaction::transaction_hashing::TransactionHasher;
use crate::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use crate::transaction::UtxoPreimageArgs;
use std::marker::PhantomData;
use tw_memory::Data;

/// `Taproot1Sighash`is used to calculate a preimage hash of a P2WPKH or P2WSH unspent output.
pub struct Taproot1Sighash<Transaction: TransactionInterface> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction: TransactionInterface> Taproot1Sighash<Transaction> {
    pub fn sighash_tx(tx: &Transaction, args: &UtxoPreimageArgs) -> UtxoResult<Data> {
        let input = tx
            .inputs()
            .get(args.input_index)
            .ok_or(UtxoError(UtxoErrorKind::Error_sighash_failed))?;

        let prevout_hash = TransactionHasher::<Transaction>::preimage_prevout_hash(tx, args);
        let sequence_hash = TransactionHasher::<Transaction>::preimage_sequence_hash(tx, args);
        let outputs_hash = TransactionHasher::<Transaction>::preimage_outputs_hash(tx, args);

        let mut stream = Stream::default();

        stream
            .append(&0) // epoch
            .append(&args.sighash_ty.raw_sighash())
            .append(&tx.version())
            .append(&tx.locktime());
            //.append_raw_slice(&prevout_hash)
            //.append_raw_slice(&sequence_hash);

        if !args.sighash_ty.anyone_can_pay() {
            stream
                .append(input.previous_output())
                .append(&args.amount)
                .append(&args.script_pubkey)
                .append(&input.sequence());
        }

        // The input being signed (replacing the scriptSig with scriptCode + amount)
        // The prevout may already be contained in hashPrevout, and the nSequence
        // may already be contained in hashSequence.

        // TODO: What about `NonePlusAnyoneCanPay`.
        if args.sighash_ty.base_type() != SighashBase::None && args.sighash_ty.base_type() != SighashBase::Single {
            stream.append_raw_slice(&outputs_hash);
        }

        // TODO: Leaf hash node
        let leaf_hash_node_is_some = false;
        let mut spend_type = 0u8;
        if leaf_hash_node_is_some {
            spend_type |= 2u8;
        }

        stream.append(&spend_type);

        if args.sighash_ty.anyone_can_pay() {
            todo!()
        } else {
            stream.append(&(args.input_index as u32));
        }

        // TODO:
        let annex_is_some = false;
        if annex_is_some {
            todo!()
        }

        stream
            .append_raw_slice(&outputs_hash)
            // This also includes 24-bit fork id. For example, 0 for BitcoinCash.
            .append(&args.sighash_ty.raw_sighash());

        Ok(args.tx_hasher.hash(&stream.out()))
    }
}
