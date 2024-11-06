// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_polkadot_address_normalization() {
    test_address_normalization(
        CoinType::Polkadot,
        "12dyy3fArMPDXLsnRtapTqZsC2KCEimeqs1dop4AEERaKC6x",
        "12dyy3fArMPDXLsnRtapTqZsC2KCEimeqs1dop4AEERaKC6x",
    );
}

#[test]
fn test_polkadot_address_is_valid() {
    test_address_valid(
        CoinType::Polkadot,
        "12dyy3fArMPDXLsnRtapTqZsC2KCEimeqs1dop4AEERaKC6x",
    );
}

#[test]
fn test_polkadot_address_invalid() {
    test_address_invalid(CoinType::Polkadot, "");
    test_address_invalid(
        CoinType::Polkadot,
        "5DhgpiQ6za7k5osGUFXpKgjiLQKYYRDWmNH9eX4og9Q48huk...",
    );
}

#[test]
fn test_polkadot_address_get_data() {
    test_address_get_data(
        CoinType::Polkadot,
        "12dyy3fArMPDXLsnRtapTqZsC2KCEimeqs1dop4AEERaKC6x",
        "0x4870d56d074c50e891506d78faa4fb69ca039cc5f131eb491e166b975880e867",
    );
}
