// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::derivation::Derivation;

pub enum BitcoinDerivation {
    Legacy,
    Segwit,
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
                if default_derivation.name == Derivation::Segwit {
                    return BitcoinDerivation::Segwit;
                }
                BitcoinDerivation::Legacy
            },
            Derivation::Segwit => BitcoinDerivation::Segwit,
            Derivation::Legacy => BitcoinDerivation::Legacy,
        }
    }

    /// TrustWallet behaviour inherited from:
    /// https://github.com/trustwallet/wallet-core/blob/b65adc4c86e49eb905f659ade025185a62e87ca9/src/Bitcoin/Entry.cpp#L14
    pub fn tw_supports_segwit(coin: &dyn CoinContext) -> bool {
        coin.derivations()
            .iter()
            .any(|der| der.name == Derivation::Segwit)
    }
}
