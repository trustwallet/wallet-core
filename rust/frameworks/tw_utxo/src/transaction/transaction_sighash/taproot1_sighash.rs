// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::sighash::SighashBase;
use crate::transaction::transaction_hashing::TransactionHasher;
use crate::transaction::transaction_interface::TransactionInterface;
use crate::transaction::UtxoTaprootPreimageArgs;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::tapsighash;
use tw_hash::H256;

/// `Taproot1Sighash`is used to calculate a preimage hash of a P2WPKH or P2WSH unspent output.
pub struct Taproot1Sighash<Transaction: TransactionInterface> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction: TransactionInterface> Taproot1Sighash<Transaction> {
    pub fn sighash_tx(tx: &Transaction, tr: &UtxoTaprootPreimageArgs) -> SigningResult<H256> {
        // TODO if anyone_can_pay flag is set, there is no need to append these hashes.
        // See https://github.com/rust-bitcoin/rust-bitcoin/blob/b0870634f0e4bd4c36e7ab0b7c9c7deb23ae62bf/bitcoin/src/crypto/sighash.rs#L608-L622
        let prevout_hash = TransactionHasher::<Transaction>::preimage_prevout_hash(tx, &tr.args);
        let sequence_hash = TransactionHasher::<Transaction>::preimage_sequence_hash(tx, &tr.args);
        let outputs_hash = TransactionHasher::<Transaction>::preimage_outputs_hash(tx, &tr.args);
        let spent_amounts_hash = TransactionHasher::<Transaction>::spent_amount_hash(tr);
        let raw_sighash = tr.args.sighash_ty.serialize_as_taproot()?;

        let spent_script_pubkeys_hash = TransactionHasher::<Transaction>::spent_script_pubkeys(tr);

        let mut stream = Stream::default();

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

        if tr.args.leaf_hash_code_separator.is_some() {
            spend_type |= 2u8;
        }

        stream.append(&spend_type);

        if tr.args.sighash_ty.anyone_can_pay() {
            return SigningError::err(SigningErrorType::Error_not_supported)
                .context("'anyone can pay' sighash type is not supported for Taproot yet");
        } else {
            stream.append(&(tr.args.input_index as u32));
        }

        if tr.args.sighash_ty.base_type() == SighashBase::Single {
            return SigningError::err(SigningErrorType::Error_not_supported)
                .context("'single' sighash type is not supported for Taproot yet");
        }

        if let Some((leaf_hash, separator)) = tr.args.leaf_hash_code_separator {
            stream
                .append_raw_slice(leaf_hash.as_slice())
                .append(&0u8) // key-version 0
                .append(&separator);
        }

        let hash = tapsighash(&stream.out());
        H256::try_from(hash.as_slice())
            .tw_err(|_| SigningErrorType::Error_internal)
            .context("Taproot sighash must be H256")
    }
}
