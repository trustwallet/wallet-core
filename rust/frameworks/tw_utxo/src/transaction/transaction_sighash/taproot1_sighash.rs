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
use tw_encoding::hex;
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
            .append(&0u8) // epoch
            .append(&(args.sighash_ty.raw_sighash() as u8))
            .append(&tx.version())
            .append(&tx.locktime());
        //.append_raw_slice(&prevout_hash)
        //.append_raw_slice(&sequence_hash);

        if !args.sighash_ty.anyone_can_pay() {
            // > sha_prevouts (32): the SHA256 of the serialization of all input outpoints.
            let mut s = Stream::default();
            s.append_list(
                &args
                    .tr_spent_outpoints
                    .iter()
                    .map(|(amount, script)| {
                        let mut s = Stream::default();
                        s.append(amount).append(script);
                        s.out()
                    })
                    .collect::<Vec<Data>>(),
            );
            let spent_outpoints = args.tx_hasher.hash(&s.out());

            // > sha_amounts (32): the SHA256 of the serialization of all spent output amounts.
            let mut s = Stream::default();
            s.append_list(&args.tr_spent_amounts);
            let spent_amounts = args.tx_hasher.hash(&s.out());

            // > sha_scriptpubkeys (32): the SHA256 of the serialization of all spent output scriptPubKeys.
            let mut s = Stream::default();
            s.append_list(&args.tr_spent_script_pubkeys);
            let spent_script_pubkeys = args.tx_hasher.hash(&s.out());

            // > sha_sequences (32): the SHA256 of the serialization of all input nSequence.
            let mut s = Stream::default();
            s.append_list(&args.tr_spent_sequences);
            let spent_sequences = args.tx_hasher.hash(&s.out());

            stream
                .append(&spent_outpoints)
                .append(&spent_amounts)
                .append(&spent_script_pubkeys)
                .append(&spent_sequences);
        }

        // TODO: What about `NonePlusAnyoneCanPay`?.
        if args.sighash_ty.base_type() != SighashBase::None
            && args.sighash_ty.base_type() != SighashBase::Single
        {
            stream.append(&outputs_hash);
        }

        let mut spend_type = 0u8;

        // TODO: Leaf hash node
        let leaf_hash_node_is_some = false;
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

        if args.sighash_ty.base_type() == SighashBase::Single {
            todo!()
        }

        if leaf_hash_node_is_some {
            todo!()
        }

        let sighash = args.tx_hasher.hash(&stream.out());
        let hex_sighash = hex::encode(sighash.as_slice(), true);
        dbg!(hex_sighash);

        Ok(sighash)
    }
}
