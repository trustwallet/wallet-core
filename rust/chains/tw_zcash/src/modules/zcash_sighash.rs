// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::ZcashTransaction;
use tw_coin_entry::error::prelude::*;
use tw_hash::blake2::{blake2_b_personal, Blake2bPersonalHasher};
use tw_hash::{H128, H256};
use tw_memory::Data;
use tw_utxo::encode::stream::Stream;
use tw_utxo::transaction::transaction_hashing::TransactionHasher;
use tw_utxo::transaction::transaction_interface::TransactionInterface;
use tw_utxo::transaction::transaction_sighash::legacy_sighash::LegacySighash;
use tw_utxo::transaction::UtxoPreimageArgs;

const DEFAULT_HASH: H256 = H256::new();
const PERSONALISATION_PREFIX: &[u8] = b"ZcashSigHash";
const PREVOUT_PERSONALISATION: &[u8] = b"ZcashPrevoutHash";
const SEQUENCE_PERSONALISATION: &[u8] = b"ZcashSequencHash";
const OUTPUTS_PERSONALISATION: &[u8] = b"ZcashOutputsHash";

type ZcashLegacySighash = LegacySighash<ZcashTransaction>;

/// Zcash sighash includes `JoinSplit`, `ShieldedSpends`, `ShieldedOutputs` hashes
/// that aren't included in the encoded transaction.
/// That's why we can't rely on [`LegacySighash`] fully, but implement a custom sighash computer.
pub struct ZcashSighash;

impl ZcashSighash {
    pub fn sighash_tx(tx: &ZcashTransaction, args: &UtxoPreimageArgs) -> SigningResult<H256> {
        let mut personalisation = H128::new();
        personalisation[..PERSONALISATION_PREFIX.len()].copy_from_slice(PERSONALISATION_PREFIX);
        personalisation[PERSONALISATION_PREFIX.len()..].copy_from_slice(tx.branch_id.as_slice());

        let preimage = Self::preimage_tx(tx, args)?;
        // Ignore `args.tx_hasher` and use `blake2_b_personal` instead.
        blake2_b_personal(preimage.as_slice(), H256::LEN, personalisation.as_slice())
            .and_then(|sighash| H256::try_from(sighash.as_slice()))
            .tw_err(SigningErrorType::Error_internal)
            .context("'blake2_b_personal' failed")
    }

    pub fn preimage_tx(tx: &ZcashTransaction, args: &UtxoPreimageArgs) -> SigningResult<Data> {
        let mut tx_preimage = tx.clone();

        tx_preimage.replace_inputs(ZcashLegacySighash::inputs_for_preimage(tx, args)?);
        tx_preimage.replace_outputs(ZcashLegacySighash::outputs_for_preimage(tx, args));

        let join_splits_hash = DEFAULT_HASH.as_slice();
        let shielded_spends = DEFAULT_HASH.as_slice();
        let shielded_outputs = DEFAULT_HASH.as_slice();

        let prevout_hash = Self::prevout_hash(&tx_preimage, args);
        let sequence_hash = Self::sequence_hash(&tx_preimage, args);
        let outputs_hash = Self::outputs_hash(&tx_preimage, args);

        let mut stream = Stream::default();

        stream.append(&tx.version);
        stream.append(&tx.version_group_id);

        stream
            .append_raw_slice(&prevout_hash)
            .append_raw_slice(&sequence_hash)
            .append_raw_slice(&outputs_hash)
            .append_raw_slice(join_splits_hash)
            .append_raw_slice(shielded_spends)
            .append_raw_slice(shielded_outputs);

        stream
            .append(&tx.locktime)
            .append(&tx.expiry_height)
            .append(&tx.sapling_value_balance)
            // Append the sighash type.
            .append(&args.sighash_ty.raw_sighash());

        // The input being signed (replacing the scriptSig with scriptCode + amount)
        // The prevout may already be contained in hashPrevout, and the nSequence
        // may already be contained in hashSequence.
        let utxo = tx
            .transparent_inputs
            .get(args.input_index)
            .or_tw_err(SigningErrorType::Error_internal)
            .context("Zcash sighash error: input_index is out of bounds")?;

        stream
            .append(&utxo.previous_output)
            .append(&args.script_pubkey)
            .append(&args.amount)
            .append(&utxo.sequence);

        Ok(stream.out())
    }

    pub fn prevout_hash(tx: &ZcashTransaction, args: &UtxoPreimageArgs) -> Data {
        let blake2b_hasher = Blake2bPersonalHasher::new(H256::LEN, PREVOUT_PERSONALISATION)
            .expect("Expected a valid Blake2b hash and personalisation lengths");
        TransactionHasher::preimage_prevout_hash(tx, args.sighash_ty, blake2b_hasher)
    }

    pub fn sequence_hash(tx: &ZcashTransaction, args: &UtxoPreimageArgs) -> Data {
        let blake2b_hasher = Blake2bPersonalHasher::new(H256::LEN, SEQUENCE_PERSONALISATION)
            .expect("Expected a valid Blake2b hash and personalisation lengths");
        TransactionHasher::preimage_sequence_hash(tx, args.sighash_ty, blake2b_hasher)
    }

    pub fn outputs_hash(tx: &ZcashTransaction, args: &UtxoPreimageArgs) -> Data {
        let blake2b_hasher = Blake2bPersonalHasher::new(H256::LEN, OUTPUTS_PERSONALISATION)
            .expect("Expected a valid Blake2b hash and personalisation lengths");
        TransactionHasher::preimage_outputs_hash(
            tx,
            args.input_index,
            args.sighash_ty,
            blake2b_hasher,
        )
    }
}
