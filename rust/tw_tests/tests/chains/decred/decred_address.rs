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
        KeyType::PublicKey("0279be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"),
        "DsmcYVbP1Nmag2H4AS17UTvmWXmGeA7nLDx",
    );
}

#[test]
fn test_decred_address_normalization() {
    test_address_normalization(
        CoinType::Decred,
        "DsmcYVbP1Nmag2H4AS17UTvmWXmGeA7nLDx",
        "DsmcYVbP1Nmag2H4AS17UTvmWXmGeA7nLDx",
    );
}

#[test]
fn test_decred_address_is_valid() {
    test_address_valid(CoinType::Decred, "DsmcYVbP1Nmag2H4AS17UTvmWXmGeA7nLDx");
    test_address_valid(CoinType::Decred, "Dcur2mcGjmENx4DhNqDctW5wJCVyT3Qeqkx");
}

#[test]
fn test_decred_address_invalid() {
    test_address_invalid(CoinType::Decred, "rnBFvgZphmN39GWzUJeUitaP22Fr9be75H");
    test_address_invalid(CoinType::Decred, "t3gQDEavk5VzAAHK8TrQu2BWDLxEiF1unBm");
}

#[test]
fn test_decred_address_get_data() {
    test_address_get_data(
        CoinType::Decred,
        "DsmcYVbP1Nmag2H4AS17UTvmWXmGeA7nLDx",
        "e280cb6e66b96679aec288b1fbdbd4db08077a1b",
    );
}
