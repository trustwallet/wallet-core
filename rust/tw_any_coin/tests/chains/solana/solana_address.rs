// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_solana_address_normalization() {
    test_address_normalization(CoinType::Solana, "DENORMALIZED", "EXPECTED");
}

#[test]
fn test_solana_address_is_valid() {
    test_address_valid(CoinType::Solana, "VALID ADDRESS");
}

#[test]
fn test_solana_address_invalid() {
    test_address_invalid(CoinType::Solana, "INVALID ADDRESS");
}

#[test]
fn test_solana_address_get_data() {
    test_address_get_data(CoinType::Solana, "ADDRESS", "HEX(DATA)");
}
