// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

/// TODO consider merging `TaprootAll` and `TaprootOnePrevout`.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum SigningMethod {
    /// Used for P2SH and P2PKH.
    Legacy,
    /// Used for P2WSH and P2WPKH.
    Segwit,
    /// Used for P2TR key-path and P2TR script-paty.
    TaprootAll,
    /// Used for P2TR key-path and P2TR script-paty if only one prevout should be
    /// used to calculate the Sighash. Normally, this is not used.
    TaprootOnePrevout,
}
