// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::sighash_computer::SpendingData;
use crate::signature::{BitcoinEcdsaSignature, BitcoinSchnorrSignature};
use std::fmt;
use std::rc::Rc;

pub mod standard_constructor;

pub trait EcdsaSpendingDataConstructor: fmt::Debug {
    fn get_spending_data(&self, sig: &BitcoinEcdsaSignature) -> SpendingData;
}

pub trait SchnorrSpendingDataConstructor: fmt::Debug {
    fn get_spending_data(&self, sig: &BitcoinSchnorrSignature) -> SpendingData;
}

/// A UTXO `SpendingData` constructor.
/// Each enum variant differs with a signature type it expects to construct `SpendingData`.
/// TODO consider adding a `UtxoContext` with a `UtxoContext::SpendingDataConstructor: Clone` associated type.
#[derive(Clone, Debug)]
pub enum SpendingDataConstructor {
    Ecdsa(Rc<dyn EcdsaSpendingDataConstructor>),
    Schnorr(Rc<dyn SchnorrSpendingDataConstructor>),
}

impl SpendingDataConstructor {
    pub fn ecdsa<T: EcdsaSpendingDataConstructor + 'static>(constructor: T) -> Self {
        SpendingDataConstructor::Ecdsa(Rc::new(constructor))
    }

    pub fn schnorr<T: SchnorrSpendingDataConstructor + 'static>(constructor: T) -> Self {
        SpendingDataConstructor::Schnorr(Rc::new(constructor))
    }
}
