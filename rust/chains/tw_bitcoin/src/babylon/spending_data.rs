// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon::covenant_committee::CovenantCommittee;
use tw_keypair::schnorr;
use tw_memory::Data;
use tw_utxo::script::standard_script::claims;
use tw_utxo::script::Script;
use tw_utxo::signature::BitcoinSchnorrSignature;
use tw_utxo::spending_data::{SchnorrSpendingDataConstructor, SpendingData};

#[derive(Clone, Debug)]
pub struct BabylonUnbondingPath {
    unbonding_script: Script,
    control_block: Data,
    /// Signatures signed by covenant committees. Sorted by covenant committees public keys.
    covenant_committee_signatures: Vec<BitcoinSchnorrSignature>,
}

impl BabylonUnbondingPath {
    pub fn new(
        unbonding_script: Script,
        control_block: Data,
        covenant_committee_signatures: &[(schnorr::XOnlyPublicKey, BitcoinSchnorrSignature)],
    ) -> Self {
        let covenant_committee_signatures =
            CovenantCommittee::sort_signatures(covenant_committee_signatures);
        BabylonUnbondingPath {
            unbonding_script,
            control_block,
            covenant_committee_signatures,
        }
    }
}

impl SchnorrSpendingDataConstructor for BabylonUnbondingPath {
    fn get_spending_data(&self, sig: &BitcoinSchnorrSignature) -> SpendingData {
        // User's signature is always first.
        // Then, covenant committee signatures sorted by their public keys.
        // For more info, see [`babylon::conditions::new_unbonding_script`].
        let unbonding_sigs: Vec<_> = std::iter::once(sig.clone())
            .chain(self.covenant_committee_signatures.iter().cloned())
            .collect();

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
