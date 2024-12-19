// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon::multi_sig_ordered::MultiSigOrdered;
use tw_memory::Data;
use tw_utxo::script::standard_script::claims;
use tw_utxo::script::Script;
use tw_utxo::signature::BitcoinSchnorrSignature;
use tw_utxo::spending_data::{SchnorrSpendingDataConstructor, SpendingData};

#[derive(Clone, Debug)]
pub struct BabylonUnbondingPath {
    unbonding_script: Script,
    control_block: Data,
    /// Signatures signed by covenant committees.
    /// Sorted by covenant committees public keys in reverse order.
    covenant_committee_signatures: MultiSigOrdered,
}

impl BabylonUnbondingPath {
    pub fn new(
        unbonding_script: Script,
        control_block: Data,
        covenant_committee_signatures: MultiSigOrdered,
    ) -> Self {
        BabylonUnbondingPath {
            unbonding_script,
            control_block,
            covenant_committee_signatures,
        }
    }
}

impl SchnorrSpendingDataConstructor for BabylonUnbondingPath {
    fn get_spending_data(&self, sig: &BitcoinSchnorrSignature) -> SpendingData {
        // Covenant committee signatures are sorted by corresponding public keys in reverse lexicographical order.
        // That's because the script interpreter will pop the left-most byte-array as the first stack element,
        // the second-left-most byte array as the second stack element, and so on.
        let mut unbonding_sigs = self
            .covenant_committee_signatures
            .get_signatures_reverse_order();
        // User's signature is always last.
        unbonding_sigs.push(Some(sig.clone()));

        SpendingData {
            script_sig: Script::default(),
            witness: claims::new_p2tr_script_path(
                &unbonding_sigs,
                self.unbonding_script.clone(),
                self.control_block.clone(),
            ),
        }
    }
}
