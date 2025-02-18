// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_derive, test_address_get_data, test_address_invalid, test_address_normalization,
    test_address_valid, KeyType,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_decred_address_derive() {
    test_address_derive(
        CoinType::Decred,
        KeyType::PrivateKey("PRIVATE_KEY"),
        "EXPECTED ADDRESS"
    );
}

#[test]
fn test_decred_address_normalization() {
    test_address_normalization(CoinType::Decred, "DENORMALIZED", "EXPECTED");
}

#[test]
fn test_decred_address_is_valid() {
    test_address_valid(CoinType::Decred, "VALID ADDRESS");
}

#[test]
fn test_decred_address_invalid() {
    test_address_invalid(CoinType::Decred, "INVALID ADDRESS");
}

#[test]
fn test_decred_address_get_data() {
    test_address_get_data(CoinType::Decred, "ADDRESS", "HEX(DATA)");
}
