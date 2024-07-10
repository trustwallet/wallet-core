// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_ton_address_normalization() {
    test_address_normalization(CoinType::TON, "DENORMALIZED", "EXPECTED");
}

#[test]
fn test_ton_address_is_valid() {
    test_address_valid(CoinType::TON, "VALID ADDRESS");
}

#[test]
fn test_ton_address_invalid() {
    test_address_invalid(CoinType::TON, "INVALID ADDRESS");
}

#[test]
fn test_ton_address_get_data() {
    test_address_get_data(CoinType::TON, "ADDRESS", "HEX(DATA)");
}
