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
    pub fn sighash_tx(tx: &Transaction, tr: &UtxoTaprootPreimageArgs) -> UtxoResult<Data> {
        let prevout_hash = TransactionHasher::<Transaction>::preimage_prevout_hash(tx, &tr.args);
        let sequence_hash = TransactionHasher::<Transaction>::preimage_sequence_hash(tx, &tr.args);
        let outputs_hash = TransactionHasher::<Transaction>::preimage_outputs_hash(tx, &tr.args);
        let spent_amounts_hash = TransactionHasher::<Transaction>::spent_amount_hash(&tr);

        let spent_script_pubkeys_hash = TransactionHasher::<Transaction>::spent_script_pubkeys(&tr);

        let mut stream = Stream::default();

        // For conventional reasons, as observed in the behavior of the
        // `rust-bitcoin` crate, we map `SighashType::All` to 0x00, which
        // indicates the default behavior. The 0x00 variant is only supported in
        // Taproot transactions, not in Legacy or Segwit transactions.
        let raw_sighash = if let SighashBase::All = tr.args.sighash_ty.base_type() {
            0u8
        } else {
            tr.args.sighash_ty.raw_sighash() as u8
        };

        stream
            .append(&0u8) // epoch
            .append(&raw_sighash)
            .append(&tx.version())
            .append(&tx.locktime())
            .append_raw_slice(&prevout_hash)
            .append_raw_slice(&spent_amounts_hash)
            .append_raw_slice(&spent_script_pubkeys_hash)
            .append_raw_slice(&sequence_hash)
            // TODO: Double check behavior or `Single`
            // TODO: What about `NonePlusAnyoneCanPay`?.
            .append_raw_slice(&outputs_hash);

        let mut spend_type = 0u8;

        // TODO: Leaf hash node
        let leaf_hash_node_is_some = false;
        if leaf_hash_node_is_some {
            spend_type |= 2u8;
        }

        stream.append(&spend_type);

        if tr.args.sighash_ty.anyone_can_pay() {
            todo!()
        } else {
            stream.append(&(tr.args.input_index as u32));
        }

        let annex_is_some = false;
        if annex_is_some {
            todo!()
        }

        if tr.args.sighash_ty.base_type() == SighashBase::Single {
            todo!()
        }

        if leaf_hash_node_is_some {
            todo!()
        }

        Ok(Hasher::TapSighash.hash(&stream.out()))
    }
}
