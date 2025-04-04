// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::{DecredTransaction, SerializeType};
use tw_coin_entry::error::prelude::*;
use tw_hash::blake::blake_256;
use tw_hash::H256;
use tw_memory::Data;
use tw_utxo::encode::stream::Stream;
use tw_utxo::transaction::transaction_interface::TransactionInterface;
use tw_utxo::transaction::transaction_sighash::legacy_sighash::LegacySighash;
use tw_utxo::transaction::UtxoPreimageArgs;

pub struct DecredSighash;

impl DecredSighash {
    pub fn sighash_tx(tx: &DecredTransaction, args: &UtxoPreimageArgs) -> SigningResult<H256> {
        let mut tx_preimage = tx.clone();

        let inputs_to_preimage = LegacySighash::inputs_for_preimage(&tx_preimage, args)?;
        let outputs_to_preimage = LegacySighash::outputs_for_preimage(&tx_preimage, args);

        tx_preimage.replace_inputs(inputs_to_preimage);
        tx_preimage.replace_outputs(outputs_to_preimage);

        let prefix_hash = Self::tx_prefix_hash(&tx_preimage);
        let witness_hash = Self::tx_witness_hash(&tx_preimage);

        let mut preimage_stream = Stream::new();
        preimage_stream
            .append(&args.sighash_ty.raw_sighash())
            .append_raw_slice(&prefix_hash)
            .append_raw_slice(&witness_hash);

        let preimage_hash = blake_256(&preimage_stream.out());
        H256::try_from(preimage_hash.as_slice())
            .tw_err(SigningErrorType::Error_internal)
            .context("'blake_256' hash should return exactly 32 bytes")
    }

    pub fn tx_prefix_hash(tx: &DecredTransaction) -> Data {
        let mut prefix_preimage = Stream::new();
        tx.encode(&mut prefix_preimage, SerializeType::NoWitness);
        blake_256(&prefix_preimage.out())
    }

    /// Transaction Witness pre-image differs from [`SerializeType::OnlyWitness`] serialization.
    /// Instead, we should encode `version | serialization_type` and spending scriptPubkey's only.
    pub fn tx_witness_hash(tx: &DecredTransaction) -> Data {
        let witnesses: Vec<_> = tx
            .inputs
            .iter()
            .map(|input| input.script_sig.clone())
            .collect();

        let mut witness_preimage = Stream::new();
        tx.encode_version_and_serialize_type(&mut witness_preimage, SerializeType::OnlyWitness);
        witness_preimage.append_list(&witnesses);

        blake_256(&witness_preimage.out())
    }
}
