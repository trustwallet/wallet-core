// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_aptos_address_normalization() {
    test_address_normalization(
        CoinType::Aptos,
        "0xf3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc",
        "0xf3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc",
    );
}

#[test]
fn test_aptos_address_is_valid() {
    test_address_valid(CoinType::Aptos, "0x1");
    test_address_valid(
        CoinType::Aptos,
        "0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b",
    );
    test_address_valid(
        CoinType::Aptos,
        "eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b",
    );
    test_address_valid(
        CoinType::Aptos,
        "19aadeca9388e009d136245b9a67423f3eee242b03142849eb4f81a4a409e59c",
    );
    test_address_valid(
        CoinType::Aptos,
        "0x777821c78442e17d82c3d7a371f42de7189e4248e529fe6eee6bca40ddbb",
    );
    test_address_valid(
        CoinType::Aptos,
        "0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175",
    );
}

#[test]
fn test_aptos_address_invalid() {
    // Empty
    test_address_invalid(CoinType::Aptos, "");
    // Invalid Hex
    test_address_invalid(
        CoinType::Aptos,
        "Seff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b",
    );
    // Too long
    test_address_invalid(
        CoinType::Aptos,
        "eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175bb",
    );
    test_address_invalid(
        CoinType::Aptos,
        "0xSeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b",
    );
}

#[test]
fn test_aptos_address_get_data() {
    test_address_get_data(
        CoinType::Aptos,
        "0x1",
        "0000000000000000000000000000000000000000000000000000000000000001",
    );
}
