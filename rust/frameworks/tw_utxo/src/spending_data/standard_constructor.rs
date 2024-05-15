// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::script::standard_script::claims;
use crate::script::{Script, Witness};
use crate::signature::{BitcoinEcdsaSignature, BitcoinSchnorrSignature};
use crate::spending_data::{
    EcdsaSpendingDataConstructor, SchnorrSpendingDataConstructor, SpendingData,
};
use tw_hash::H264;
use tw_memory::Data;

#[derive(Clone, Debug)]
pub struct P2PKH {
    pub pubkey: H264,
}

impl EcdsaSpendingDataConstructor for P2PKH {
    fn get_spending_data(&self, sig: &BitcoinEcdsaSignature) -> SpendingData {
        SpendingData {
            script_sig: claims::new_p2pkh(sig, &self.pubkey),
            witness: Witness::default(),
        }
    }
}

#[derive(Clone, Debug)]
pub struct P2PK;

impl EcdsaSpendingDataConstructor for P2PK {
    fn get_spending_data(&self, sig: &BitcoinEcdsaSignature) -> SpendingData {
        SpendingData {
            script_sig: claims::new_p2pk(sig),
            witness: Witness::default(),
        }
    }
}

#[derive(Clone, Debug)]
pub struct P2WPKH {
    pub pubkey: H264,
}

impl EcdsaSpendingDataConstructor for P2WPKH {
    fn get_spending_data(&self, sig: &BitcoinEcdsaSignature) -> SpendingData {
        SpendingData {
            script_sig: Script::default(),
            witness: claims::new_p2wpkh(sig, self.pubkey),
        }
    }
}

#[derive(Clone, Debug)]
pub struct P2TRKeyPath;

impl SchnorrSpendingDataConstructor for P2TRKeyPath {
    fn get_spending_data(&self, sig: &BitcoinSchnorrSignature) -> SpendingData {
        SpendingData {
            script_sig: Script::default(),
            witness: claims::new_p2tr_key_path(sig.serialize()),
        }
    }
}

#[derive(Clone, Debug)]
pub struct P2TRScriptPath {
    pub payload: Script,
    pub control_block: Data,
}

impl SchnorrSpendingDataConstructor for P2TRScriptPath {
    fn get_spending_data(&self, sig: &BitcoinSchnorrSignature) -> SpendingData {
        SpendingData {
            script_sig: Script::default(),
            witness: claims::new_p2tr_script_path(
                sig,
                self.payload.clone(),
                self.control_block.clone(),
            ),
        }
    }
}
