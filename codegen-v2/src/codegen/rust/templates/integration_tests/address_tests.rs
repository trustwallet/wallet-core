// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_{COIN_ID}_address_normalization() {
    test_address_normalization(CoinType::{COIN_TYPE}, "DENORMALIZED", "EXPECTED");
}

#[test]
fn test_{COIN_ID}_address_is_valid() {
    test_address_valid(CoinType::{COIN_TYPE}, "VALID ADDRESS");
}

#[test]
fn test_{COIN_ID}_address_invalid() {
    test_address_invalid(CoinType::{COIN_TYPE}, "INVALID ADDRESS");
}

#[test]
fn test_{COIN_ID}_address_get_data() {
    test_address_get_data(CoinType::{COIN_TYPE}, "ADDRESS", "HEX(DATA)");
}
