// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_sui_address_normalization() {
    test_address_normalization(CoinType::Sui, "DENORMALIZED", "EXPECTED");
}

#[test]
fn test_sui_address_is_valid() {
    test_address_valid(CoinType::Sui, "VALID ADDRESS");
}

#[test]
fn test_sui_address_invalid() {
    test_address_invalid(CoinType::Sui, "INVALID ADDRESS");
}

#[test]
fn test_sui_address_get_data() {
    test_address_get_data(CoinType::Sui, "ADDRESS", "HEX(DATA)");
}
