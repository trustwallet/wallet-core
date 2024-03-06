// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

/// Extend this enum.
#[derive(Default)]
#[repr(u32)]
pub enum Derivation {
    /// Default derivation.
    #[default]
    Default = 0,
    Solana = 6,
}

impl Derivation {
    #[inline]
    pub fn from_raw(derivation: u32) -> Option<Derivation> {
        match derivation {
            0 => Some(Derivation::Default),
            6 => Some(Derivation::Solana),
            _ => None,
        }
    }
}
