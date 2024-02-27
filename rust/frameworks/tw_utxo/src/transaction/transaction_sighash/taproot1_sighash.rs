// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::error::{UtxoError, UtxoErrorKind, UtxoResult};
use crate::sighash::{SighashBase, SighashType};
use crate::transaction::transaction_hashing::TransactionHasher;
use crate::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use crate::transaction::{UtxoPreimageArgs, UtxoTaprootPreimageArgs};
use bitcoin::{amount, script};
use secp256k1::rand::seq;
use std::marker::PhantomData;
use tw_encoding::hex;
use tw_hash::hasher::Hasher;
use tw_memory::Data;

/// `Taproot1Sighash`is used to calculate a preimage hash of a P2WPKH or P2WSH unspent output.
pub struct Taproot1Sighash<Transaction: TransactionInterface> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction: TransactionInterface> Taproot1Sighash<Transaction> {
    pub fn sighash_tx(
        tx: &Transaction,
        taproot_args: &UtxoTaprootPreimageArgs,
    ) -> UtxoResult<Data> {
        let args = &taproot_args.preimage;
        let spent_amounts = &taproot_args.spent_amounts;
        let spent_script_pubkeys = &taproot_args.spent_script_pubkeys;

        let prevout_hash = TransactionHasher::<Transaction>::preimage_prevout_hash(tx, args);
        let sequence_hash = TransactionHasher::<Transaction>::preimage_sequence_hash(tx, args);
        let outputs_hash = TransactionHasher::<Transaction>::preimage_outputs_hash(tx, args);

        let mut stream = Stream::default();

        stream
            .append(&0u8) // epoch
            // TODO??
            //.append(&(args.sighash_ty.raw_sighash() as u8))
            .append(&0u8)
            .append(&tx.version())
            .append(&tx.locktime());

        if !args.sighash_ty.anyone_can_pay() {
            let spent_amounts_hash =
                TransactionHasher::<Transaction>::spent_amount_hash(&spent_amounts, args.tx_hasher);

            let spent_script_pubkeys_hash = TransactionHasher::<Transaction>::spent_script_pubkeys(
                &spent_script_pubkeys,
                args.tx_hasher,
            );

            stream
                .append_raw_slice(&prevout_hash)
                .append_raw_slice(&spent_amounts_hash)
                .append_raw_slice(&spent_script_pubkeys_hash)
                .append_raw_slice(&sequence_hash);
        }

        // TODO: What about `NonePlusAnyoneCanPay`?.
        if args.sighash_ty.base_type() != SighashBase::None
            && args.sighash_ty.base_type() != SighashBase::Single
        {
            stream.append_raw_slice(&outputs_hash);
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

        Ok(Hasher::TapSighash.hash(&stream.out()))
    }
}
