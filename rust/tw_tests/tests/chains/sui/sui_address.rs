// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_sui_address_normalization() {
    test_address_normalization(
        CoinType::Sui,
        "259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015",
        "0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015",
    );
}

#[test]
fn test_sui_address_is_valid() {
    test_address_valid(
        CoinType::Sui,
        "0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015",
    );
}

#[test]
fn test_sui_address_invalid() {
    // Address 20 are invalid in SUI.
    test_address_invalid(CoinType::Sui, "0xb1dc06bd64d4e179a482b97bb68243f6c02c1b92");
    test_address_invalid(CoinType::Sui, "b1dc06bd64d4e179a482b97bb68243f6c02c1b92");
    // Too long.
    test_address_invalid(
        CoinType::Sui,
        "d575ad7f18e948462a5cf698f564ef394a752a71fec62493af8a055c012c0d502",
    );
    // Too short.
    test_address_invalid(CoinType::Sui, "b1dc06bd64d4e179a482b97bb68243f6c02c1b9");
    // Invalid short address.
    test_address_invalid(CoinType::Sui, "0x11");
    // Invalid Hex
    test_address_invalid(
        CoinType::Sui,
        "0xS59ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015",
    );
}

#[test]
fn test_sui_address_get_data() {
    test_address_get_data(
        CoinType::Sui,
        "0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015",
        "259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015",
    );
}
