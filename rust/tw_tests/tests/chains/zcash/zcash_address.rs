// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_derive, test_address_get_data, test_address_invalid, test_address_normalization,
    test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_zcash_address_derive() {
    test_address_derive(CoinType::Zcash, "PRIVATE_KEY", "EXPECTED ADDRESS");
}

#[test]
fn test_zcash_address_normalization() {
    test_address_normalization(CoinType::Zcash, "DENORMALIZED", "EXPECTED");
}

#[test]
fn test_zcash_address_is_valid() {
    test_address_valid(CoinType::Zcash, "VALID ADDRESS");
}

#[test]
fn test_zcash_address_invalid() {
    test_address_invalid(CoinType::Zcash, "INVALID ADDRESS");
}

#[test]
fn test_zcash_address_get_data() {
    test_address_get_data(CoinType::Zcash, "ADDRESS", "HEX(DATA)");
}
