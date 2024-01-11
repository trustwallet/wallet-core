// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_thorchain_address_normalization() {
    test_address_normalization(
        CoinType::InternetComputer,
        "290CC7C359F44C8516FC169C5ED4F0F3AE2E24BF5DE0D4C51F5E7545B5474FAA",
        "290cc7c359f44c8516fc169c5ed4f0f3ae2e24bf5de0d4c51f5e7545b5474faa",
    );
}

#[test]
fn test_thorchain_address_is_valid() {
    test_address_valid(
        CoinType::InternetComputer,
        "fb257577279ecac604d4780214af95aa6adc3a814f6f3d6d7ac844d1deca500a",
    );
    test_address_valid(
        CoinType::InternetComputer,
        "e90c48d54847f4758f1d6b589a1db2500757a49a6722d4f775e050107b4b752d",
    );
    test_address_valid(
        CoinType::InternetComputer,
        "a7c5baf393aed527ef6fb3869fbf84dd4e562edf9b04bd8f9bfbd6b8e6a22776",
    );
    test_address_valid(
        CoinType::InternetComputer,
        "4cb2ca5cfcfa1d952f8cd7f0ec46c96e1023ab057b83a2c7ce236b9e71ccca0b",
    );
}

#[test]
fn test_thorchain_address_invalid() {
    test_address_invalid(
        CoinType::InternetComputer,
        "3357cba483f268d044d4bbd4639f82c16028a76eebdf62c51bc11fc918d278b",
    );
    test_address_invalid(
        CoinType::InternetComputer,
        "3357cba483f268d044d4bbd4639f82c16028a76eebdf62c51bc11fc918d278bce",
    );
    test_address_invalid(
        CoinType::InternetComputer,
        "553357cba483f268d044d4bbd4639f82c16028a76eebdf62c51bc11fc918d278",
    );
}

#[test]
fn test_thorchain_address_get_data() {
    test_address_get_data(
        CoinType::InternetComputer,
        "4cb2ca5cfcfa1d952f8cd7f0ec46c96e1023ab057b83a2c7ce236b9e71ccca0b",
        "4cb2ca5cfcfa1d952f8cd7f0ec46c96e1023ab057b83a2c7ce236b9e71ccca0b",
    );
}
