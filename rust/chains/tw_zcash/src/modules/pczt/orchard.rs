// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::{Deserialize, Serialize};

/// PCZT fields that are specific to producing the transaction's Orchard bundle (if any).
#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct Bundle {
    /// The Orchard actions in this bundle.
    /// We do not support Orchard UTXOs.
    /// Array must be empty to be deserialized correctly.
    pub actions: Vec<()>,

    /// The flags for the Orchard bundle.
    ///
    /// Contains:
    /// - `enableSpendsOrchard` flag (bit 0)
    /// - `enableOutputsOrchard` flag (bit 1)
    /// - Reserved, zeros (bits 2..=7)
    ///
    /// This is set by the Creator. The Constructor MUST only add spends and outputs that
    /// are consistent with these flags (i.e. are dummies as appropriate).
    pub flags: u8,

    /// The net value of Orchard spends minus outputs.
    ///
    /// This is initialized by the Creator, and updated by the Constructor as spends or
    /// outputs are added to the PCZT. It enables per-spend and per-output values to be
    /// redacted from the PCZT after they are no longer necessary.
    pub value_sum: (u64, bool),

    /// The Orchard anchor for this transaction.
    ///
    /// Set by the Creator.
    pub anchor: [u8; 32],

    /// The Orchard bundle proof.
    ///
    /// This is `None` until it is set by the Prover.
    pub zkproof: Option<Vec<u8>>,

    /// The Orchard binding signature signing key.
    ///
    /// - This is `None` until it is set by the IO Finalizer.
    /// - The Transaction Extractor uses this to produce the binding signature.
    pub bsk: Option<[u8; 32]>,
}
