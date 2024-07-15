// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum SigningMethod {
    /// Used for P2SH and P2PKH.
    Legacy,
    /// Used for P2WSH and P2WPKH.
    Segwit,
    /// Used for P2TR key-path and P2TR script-paty.
    Taproot,
}
