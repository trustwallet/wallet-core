// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_polkadot_address_normalization() {
    test_address_normalization(CoinType::Polkadot, "DENORMALIZED", "EXPECTED");
}

#[test]
fn test_polkadot_address_is_valid() {
    test_address_valid(CoinType::Polkadot, "VALID ADDRESS");
}

#[test]
fn test_polkadot_address_invalid() {
    test_address_invalid(CoinType::Polkadot, "INVALID ADDRESS");
}

#[test]
fn test_polkadot_address_get_data() {
    test_address_get_data(CoinType::Polkadot, "ADDRESS", "HEX(DATA)");
}
