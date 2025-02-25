// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use bitcoin::psbt::Psbt;
use tw_utxo::transaction::transaction_interface::{TransactionInterface, TxInputInterface};

/// Finalizes the [Partially Signed Bitcoin Transaction](Psbt)
/// by updating the final `script_sig` and/or `witness`.
pub fn update_psbt_signed<Transaction: TransactionInterface>(
    psbt: &mut Psbt,
    signed_tx: &Transaction,
) {
    for (signed_txin, utxo_psbt) in signed_tx.inputs().iter().zip(psbt.inputs.iter_mut()) {
        if signed_txin.has_script_sig() {
            utxo_psbt.final_script_sig = Some(bitcoin::ScriptBuf::from_bytes(
                signed_txin.script_sig().to_vec(),
            ));
        }

        if let Some(witness) = signed_txin.witness() {
            if witness.is_empty() {
                continue;
            }

            let mut final_witness = bitcoin::Witness::new();
            for witness_item in witness.as_items() {
                final_witness.push(bitcoin::ScriptBuf::from_bytes(witness_item.to_vec()));
            }
            utxo_psbt.final_script_witness = Some(final_witness);
        }
    }
}
