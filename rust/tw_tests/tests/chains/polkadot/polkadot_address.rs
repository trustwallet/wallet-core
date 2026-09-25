// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_create_ss58_with_public_key, test_address_get_data, test_address_invalid,
    test_address_normalization, test_address_ss58_is_valid, test_address_valid,
    AddressCreateSS58WithPublicKey,
};
use tw_coin_registry::coin_type::CoinType;
use tw_keypair::tw::PublicKeyType;

#[test]
fn test_polkadot_address_normalization() {
    test_address_normalization(
        CoinType::Polkadot,
        "12dyy3fArMPDXLsnRtapTqZsC2KCEimeqs1dop4AEERaKC6x",
        "12dyy3fArMPDXLsnRtapTqZsC2KCEimeqs1dop4AEERaKC6x",
    );
}

#[test]
fn test_polkadot_address_is_valid() {
    test_address_valid(
        CoinType::Polkadot,
        "12dyy3fArMPDXLsnRtapTqZsC2KCEimeqs1dop4AEERaKC6x",
    );
    test_address_ss58_is_valid(
        CoinType::Polkadot,
        "12dyy3fArMPDXLsnRtapTqZsC2KCEimeqs1dop4AEERaKC6x",
        0,
    );

    // Polkadot ed25519
    test_address_valid(
        CoinType::Polkadot,
        "15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu",
    );
    // Polkadot sr25519
    test_address_valid(
        CoinType::Polkadot,
        "15AeCjMpcSt3Fwa47jJBd7JzQ395Kr2cuyF5Zp4UBf1g9ony",
    );

    test_address_ss58_is_valid(
        CoinType::Polkadot,
        "cEYtw6AVMB27hFUs4gVukajLM7GqxwxUfJkbPY3rNToHMcCgb",
        64,
    );
}

#[test]
fn test_polkadot_address_invalid() {
    test_address_invalid(CoinType::Polkadot, "");
    test_address_invalid(
        CoinType::Polkadot,
        "5DhgpiQ6za7k5osGUFXpKgjiLQKYYRDWmNH9eX4og9Q48huk...",
    );

    // Substrate ed25519
    test_address_invalid(
        CoinType::Polkadot,
        "5FqqU2rytGPhcwQosKRtW1E3ha6BJKAjHgtcodh71dSyXhoZ",
    );
    // Bitcoin
    test_address_invalid(CoinType::Polkadot, "1ES14c7qLb5CYhLMUekctxLgc1FV2Ti9DA");
    // Kusama ed25519
    test_address_invalid(
        CoinType::Polkadot,
        "FHKAe66mnbk8ke8zVWE9hFVFrJN1mprFPVmD5rrevotkcDZ",
    );
    // Kusama secp256k1
    test_address_invalid(
        CoinType::Polkadot,
        "FxQFyTorsjVsjjMyjdgq8w5vGx8LiA1qhWbRYcFijxKKchx",
    );
    // Kusama sr25519
    test_address_invalid(
        CoinType::Polkadot,
        "EJ5UJ12GShfh7EWrcNZFLiYU79oogdtXFUuDDZzk7Wb2vCe",
    );
}

#[test]
fn test_polkadot_address_get_data() {
    test_address_get_data(
        CoinType::Polkadot,
        "12dyy3fArMPDXLsnRtapTqZsC2KCEimeqs1dop4AEERaKC6x",
        "0x4870d56d074c50e891506d78faa4fb69ca039cc5f131eb491e166b975880e867",
    );
}

#[test]
fn test_polkadot_address_create_ss58_with_public_key() {
    test_address_create_ss58_with_public_key(AddressCreateSS58WithPublicKey {
        coin: CoinType::Polkadot,
        public_key: "0x92fd9c237030356e26cfcc4568dc71055d5ec92dfe0ff903767e00611971bad3",
        public_key_type: PublicKeyType::Ed25519,
        // Kusama SS58.
        ss58: 2,
        expected: "Fu3r514w83euSVV7q1MyFGWErUR2xDzXS2goHzimUn4S12D",
    });
}
