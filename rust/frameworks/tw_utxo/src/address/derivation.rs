// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::derivation::{ChildIndex, Derivation};

pub const SEGWIT_DERIVATION_PATH_TYPE: ChildIndex = ChildIndex::Hardened(84);

pub enum BitcoinDerivation {
    Legacy,
    Segwit,
}

impl BitcoinDerivation {
    /// TrustWallet derivation inherited from:
    /// https://github.com/trustwallet/wallet-core/blob/b65adc4c86e49eb905f659ade025185a62e87ca9/src/Bitcoin/Entry.cpp#L67
    pub fn tw_derivation(coin: &dyn CoinContext, derivation: Derivation) -> BitcoinDerivation {
        match derivation {
            // In case of a default derivation specified by the function caller,
            // we should check the default derivation in the `coin`'s context.
            // Please note that testnet derivation is no longer supported. Instead, use address prefix.
            Derivation::Default | Derivation::Testnet => (),
            Derivation::Segwit => return BitcoinDerivation::Segwit,
            Derivation::Legacy => return BitcoinDerivation::Legacy,
        }

        let Some(default_derivation) = coin.derivations().first() else {
            return BitcoinDerivation::Legacy;
        };
        let derivation_path_type = default_derivation.path.path().first().copied();

        match default_derivation.name {
            Derivation::Segwit => BitcoinDerivation::Segwit,
            Derivation::Default if derivation_path_type == Some(SEGWIT_DERIVATION_PATH_TYPE) => {
                BitcoinDerivation::Segwit
            },
            Derivation::Default | Derivation::Legacy | Derivation::Testnet => {
                BitcoinDerivation::Legacy
            },
        }
    }

    /// TrustWallet behaviour inherited from:
    /// https://github.com/trustwallet/wallet-core/blob/b65adc4c86e49eb905f659ade025185a62e87ca9/src/Bitcoin/Entry.cpp#L14
    pub fn tw_supports_segwit(coin: &dyn CoinContext) -> bool {
        coin.derivations().iter().any(|der| {
            der.name == Derivation::Segwit
                || der.path.path().first().copied() == Some(SEGWIT_DERIVATION_PATH_TYPE)
        })
    }
}
