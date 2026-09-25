// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_bech32_is_valid, test_address_create_bech32_with_public_key,
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
    AddressBech32IsValid, AddressCreateBech32WithPublicKey,
};
use tw_coin_registry::coin_type::CoinType;
use tw_keypair::tw::PublicKeyType;

#[test]
fn test_cosmos_address_normalization() {
    test_address_normalization(
        CoinType::Cosmos,
        "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08",
        "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08",
    );
}

#[test]
fn test_cosmos_address_is_valid() {
    test_address_valid(
        CoinType::Cosmos,
        "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
    );
    test_address_valid(
        CoinType::Cosmos,
        "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08",
    );
    test_address_valid(
        CoinType::Cosmos,
        "cosmosvalconspub1zcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa",
    );
}

#[test]
fn test_cosmos_address_invalid() {
    test_address_invalid(
        CoinType::Cosmos,
        "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax6",
    );
    test_address_invalid(
        CoinType::Cosmos,
        "one1a50tun737ulcvwy0yvve0pvu5skq0kjargvhwe",
    );
    test_address_invalid(
        CoinType::Cosmos,
        "bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2",
    );
}

#[test]
fn test_cosmos_address_get_data() {
    test_address_get_data(
        CoinType::Cosmos,
        "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08",
        "818c5dc04ccbd1dcb454fb7c06da564b0e22955d",
    );
}

#[test]
fn test_any_address_is_valid_bech32() {
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Cosmos,
        address: "juno1mry47pkga5tdswtluy0m8teslpalkdq0gnn4mf",
        hrp: "juno",
    });
}

#[test]
fn test_any_address_create_bech32_with_public_key() {
    test_address_create_bech32_with_public_key(AddressCreateBech32WithPublicKey {
        coin: CoinType::Cosmos,
        private_key: "a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433",
        public_key_type: PublicKeyType::Secp256k1,
        hrp: "dydx",
        expected: "dydx1mry47pkga5tdswtluy0m8teslpalkdq0hc72uz",
    });
}
