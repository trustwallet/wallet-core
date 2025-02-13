// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_derive, test_address_get_data, test_address_invalid, test_address_normalization,
    test_address_valid, KeyType,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_groestlcoin_address_derive() {
    test_address_derive(
        CoinType::Groestlcoin,
        KeyType::PrivateKey("PRIVATE_KEY"),
        "EXPECTED ADDRESS"
    );
}

#[test]
fn test_groestlcoin_address_normalization() {
    test_address_normalization(CoinType::Groestlcoin, "DENORMALIZED", "EXPECTED");
}

#[test]
fn test_groestlcoin_address_is_valid() {
    test_address_valid(CoinType::Groestlcoin, "VALID ADDRESS");
}

#[test]
fn test_groestlcoin_address_invalid() {
    test_address_invalid(CoinType::Groestlcoin, "INVALID ADDRESS");
}

#[test]
fn test_groestlcoin_address_get_data() {
    test_address_get_data(CoinType::Groestlcoin, "ADDRESS", "HEX(DATA)");
}
