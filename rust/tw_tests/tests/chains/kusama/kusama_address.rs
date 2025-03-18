// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{test_address_invalid, test_address_valid};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_kusama_address_is_valid() {
    // Kusama ed25519
    test_address_valid(
        CoinType::Kusama,
        "FHKAe66mnbk8ke8zVWE9hFVFrJN1mprFPVmD5rrevotkcDZ",
    );
    // Kusama secp256k1
    test_address_valid(
        CoinType::Kusama,
        "FxQFyTorsjVsjjMyjdgq8w5vGx8LiA1qhWbRYcFijxKKchx",
    );
    // Kusama sr25519
    test_address_valid(
        CoinType::Kusama,
        "EJ5UJ12GShfh7EWrcNZFLiYU79oogdtXFUuDDZzk7Wb2vCe",
    );
}

#[test]
fn test_kusama_address_invalid() {
    // Substrate ed25519
    test_address_invalid(
        CoinType::Kusama,
        "5FqqU2rytGPhcwQosKRtW1E3ha6BJKAjHgtcodh71dSyXhoZ",
    );
    // Polkadot ed25519
    test_address_invalid(
        CoinType::Kusama,
        "15AeCjMpcSt3Fwa47jJBd7JzQ395Kr2cuyF5Zp4UBf1g9ony",
    );
    // Polkadot sr25519
    test_address_invalid(
        CoinType::Kusama,
        "15AeCjMpcSt3Fwa47jJBd7JzQ395Kr2cuyF5Zp4UBf1g9ony",
    );
    // Bitcoin
    test_address_invalid(CoinType::Kusama, "1ES14c7qLb5CYhLMUekctxLgc1FV2Ti9DA");
}
