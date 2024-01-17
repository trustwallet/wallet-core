// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_bech32_is_valid, test_address_get_data, test_address_invalid,
    test_address_normalization, test_address_valid, AddressBech32IsValid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_binance_address_normalization() {
    test_address_normalization(
        CoinType::Binance,
        "bnb1h8xf9htasu9aclra954dnnve8fgcda4ae7qfa8",
        "bnb1h8xf9htasu9aclra954dnnve8fgcda4ae7qfa8",
    );
}

#[test]
fn test_binance_address_is_valid() {
    test_address_valid(
        CoinType::Binance,
        "bnb1h8xf9htasu9aclra954dnnve8fgcda4ae7qfa8",
    );
    // Validator address.
    test_address_valid(
        CoinType::Binance,
        "bva10npy5809y303f227g4leqw7vs3s6ep5ul26sq2",
    );
}

#[test]
fn test_binance_address_invalid() {
    // Testnet address.
    test_address_invalid(
        CoinType::Binance,
        "tbnb1x4hxmtdf7pwea9dghq73dufh3qspm8gp5qht9c",
    );
    // Unknown `bfa` hrp.
    test_address_invalid(
        CoinType::Binance,
        "bfa10npy5809y303f227g4leqw7vs3s6ep5ul26sq2",
    );
}

#[test]
fn test_binance_address_get_data() {
    test_address_get_data(
        CoinType::Binance,
        "bnb1h8xf9htasu9aclra954dnnve8fgcda4ae7qfa8",
        "b9cc92dd7d870bdc7c7d2d2ad9cd993a5186f6bd",
    );
}

#[test]
fn test_binance_address_is_valid_bech32() {
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Binance,
        address: "bva10npy5809y303f227g4leqw7vs3s6ep5ul26sq2",
        hrp: "bva",
    });
}
