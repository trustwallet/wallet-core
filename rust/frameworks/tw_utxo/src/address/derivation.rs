// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::derivation::{ChildIndex, Derivation};

pub const SEGWIT_PURPOSE: ChildIndex = ChildIndex::Hardened(84);

pub enum BitcoinDerivation {
    Legacy,
    Segwit,
    Taproot,
}

impl BitcoinDerivation {
    /// TrustWallet derivation inherited from:
    /// https://github.com/trustwallet/wallet-core/blob/b65adc4c86e49eb905f659ade025185a62e87ca9/src/Bitcoin/Entry.cpp#L67
    pub fn tw_derivation(coin: &dyn CoinContext, derivation: Derivation) -> BitcoinDerivation {
        match derivation {
            Derivation::Default
            // Please note that testnet derivation is no longer supported. Instead, use address prefix.
            | Derivation::Testnet => {
                let Some(default_derivation) = coin.derivations().first() else {
                    return BitcoinDerivation::Segwit;
                };
                let Some(default_purpose) = default_derivation.path.path().first() else {
                    return BitcoinDerivation::Segwit;
                };
                if *default_purpose == SEGWIT_PURPOSE {
                    return BitcoinDerivation::Segwit;
                }
                BitcoinDerivation::Legacy
            },
            Derivation::Segwit => BitcoinDerivation::Segwit,
            Derivation::Legacy => BitcoinDerivation::Legacy,
        }
    }
}
