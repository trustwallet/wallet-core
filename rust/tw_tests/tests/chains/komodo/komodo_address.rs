// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_derive, test_address_get_data, test_address_invalid, test_address_normalization,
    test_address_valid, KeyType,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_komodo_address_derive() {
    test_address_derive(
        CoinType::Komodo,
        KeyType::PublicKey("020e46e79a2a8d12b9b5d12c7a91adb4e454edfae43c0a0cb805427d2ac7613fd9"),
        "RXL3YXG2ceaB6C5hfJcN4fvmLH2C34knhA",
    );
}

#[test]
fn test_komodo_address_normalization() {
    test_address_normalization(
        CoinType::Komodo,
        "RXL3YXG2ceaB6C5hfJcN4fvmLH2C34knhA",
        "RXL3YXG2ceaB6C5hfJcN4fvmLH2C34knhA",
    );
}

#[test]
fn test_komodo_address_is_valid() {
    test_address_valid(CoinType::Komodo, "RXL3YXG2ceaB6C5hfJcN4fvmLH2C34knhA");
}

#[test]
fn test_komodo_address_invalid() {
    test_address_invalid(CoinType::Komodo, "t1Wg9uPPAfwhBWeRjtDPa5ZHNzyBx9rJVKY");
}

#[test]
fn test_komodo_address_get_data() {
    test_address_get_data(
        CoinType::Komodo,
        "RALiENnMMjyubc38hM31h6oicPsuWdAMYg",
        "0ba28b3eebfe1d39dab038324be2c66090ee21a3",
    );
}
