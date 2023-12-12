// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_greenfield_address_normalization() {
    test_address_normalization(CoinType::Greenfield, "DENORMALIZED", "EXPECTED");
}

#[test]
fn test_greenfield_address_is_valid() {
    test_address_valid(CoinType::Greenfield, "VALID ADDRESS");
}

#[test]
fn test_greenfield_address_invalid() {
    test_address_invalid(CoinType::Greenfield, "INVALID ADDRESS");
}

#[test]
fn test_greenfield_address_get_data() {
    test_address_get_data(CoinType::Greenfield, "ADDRESS", "HEX(DATA)");
}
