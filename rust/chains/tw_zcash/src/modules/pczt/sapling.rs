// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::{Deserialize, Serialize};

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct Bundle {
    /// We do not support Sapling UTXOs.
    /// Array must be empty to be deserialized correctly.
    pub spends: Vec<()>,
    /// We do not support Sapling UTXOs.
    /// Array must be empty to be deserialized correctly.
    pub outputs: Vec<()>,

    /// The net value of Sapling spends minus outputs.
    ///
    /// This is initialized by the Creator, and updated by the Constructor as spends or
    /// outputs are added to the PCZT. It enables per-spend and per-output values to be
    /// redacted from the PCZT after they are no longer necessary.
    pub value_sum: i128,

    /// The Sapling anchor for this transaction.
    ///
    /// Set by the Creator.
    pub anchor: [u8; 32],

    /// The Sapling binding signature signing key.
    ///
    /// - This is `None` until it is set by the IO Finalizer.
    /// - The Transaction Extractor uses this to produce the binding signature.
    pub bsk: Option<[u8; 32]>,
}
