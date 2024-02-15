// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::error::{UtxoError, UtxoErrorKind, UtxoResult};
use crate::script::Script;
use crate::sighash::SighashBase;
use crate::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use crate::transaction::UtxoPreimageArgs;
use std::marker::PhantomData;
use tw_memory::Data;

/// `LegacySighash` is used to calculate a preimage hash of a P2PK, P2PKH or P2SH unspent output.
pub struct LegacySighash<Transaction: std::fmt::Debug + TransactionInterface> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction: std::fmt::Debug + TransactionInterface> LegacySighash<Transaction> {
    pub fn sighash_tx(tx: &Transaction, args: &UtxoPreimageArgs) -> UtxoResult<Data> {
        // TODO: Avoid cloning here?
        let mut tx_preimage = tx.clone();

        tx_preimage.replace_inputs(Self::inputs_for_preimage(tx, args)?);
        tx_preimage.replace_outputs(Self::outputs_for_preimage(tx, args));

        let mut stream = Stream::default();
        stream
            // Encode the transaction preimage as a normal tx.
            .append(&tx_preimage)
            // Append the sighash type.
            .append(&args.sighash_ty.raw_sighash());

        Ok(args.tx_hasher.hash(&stream.out()))
    }

    /// Select and prepare transaction inputs according to the preimage settings.
    pub fn inputs_for_preimage(
        tx: &Transaction,
        args: &UtxoPreimageArgs,
    ) -> UtxoResult<Vec<Transaction::Input>> {
        let original_inputs = tx.inputs();
        // Get an input needs to be signed.
        let input_to_sign = original_inputs
            .get(args.input_index)
            .ok_or(UtxoError(UtxoErrorKind::Error_internal))?;

        if args.sighash_ty.anyone_can_pay() {
            let mut input_preimage = input_to_sign.clone();
            input_preimage.set_script_sig(args.script_pubkey.clone());
            input_preimage.clear_witness();
            return Ok(vec![input_preimage]);
        }

        let selected_inputs = original_inputs
            .iter()
            .enumerate()
            .map(|(n, input)| {
                let is_this_input = n == args.input_index;

                let mut input_preimage = input.clone();
                input_preimage.set_script_sig(if is_this_input {
                    args.script_pubkey.clone()
                } else {
                    Script::default()
                });

                let single_or_none = matches!(
                    args.sighash_ty.base_type(),
                    SighashBase::Single | SighashBase::None
                );
                // Override the value with zero if necessary.
                if !is_this_input && single_or_none {
                    input_preimage.set_sequence(0);
                }
                input_preimage.clear_witness();
                input_preimage
            })
            .collect();
        Ok(selected_inputs)
    }

    /// Selects transaction outputs according to the preimage settings.
    pub fn outputs_for_preimage(
        tx: &Transaction,
        args: &UtxoPreimageArgs,
    ) -> Vec<Transaction::Output> {
        match args.sighash_ty.base_type() {
            // Hash all the transaction outputs.
            SighashBase::All => tx.outputs().to_vec(),
            SighashBase::Single => tx
                .outputs()
                .iter()
                .take(args.input_index + 1)
                .enumerate()
                .map(|(n, out)| {
                    if n == args.input_index {
                        out.clone()
                    } else {
                        // TODO: The output amount in this case must be `0xffffffffffffffff`.
                        // `standard_transaction::TransactionOutput` defaults to `-1`.
                        Transaction::Output::default()
                    }
                })
                .collect(),
            SighashBase::None => Vec::default(),
        }
    }
}
