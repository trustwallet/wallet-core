// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_greenfield_address_normalization() {
    test_address_normalization(
        CoinType::Greenfield,
        "0xb16db98b365b1f89191996942612b14f1da4bd5f",
        "0xb16Db98B365B1f89191996942612B14F1Da4Bd5f",
    );
}

#[test]
fn test_greenfield_address_is_valid() {
    test_address_valid(
        CoinType::Greenfield,
        "0xb16db98b365b1f89191996942612b14f1da4bd5f",
    );
}

#[test]
fn test_greenfield_address_invalid() {
    test_address_invalid(
        CoinType::Greenfield,
        "b16Db98B365B1f89191996942612B14F1Da4Bd5f",
    );
    test_address_invalid(
        CoinType::Greenfield,
        "bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2",
    );
}

#[test]
fn test_greenfield_address_get_data() {
    test_address_get_data(
        CoinType::Greenfield,
        "0xb16Db98B365B1f89191996942612B14F1Da4Bd5f",
        "b16Db98B365B1f89191996942612B14F1Da4Bd5f",
    );
}
