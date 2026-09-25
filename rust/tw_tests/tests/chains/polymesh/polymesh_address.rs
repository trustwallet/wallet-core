// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::polymesh::{PRIVATE_KEY_1, PUBLIC_KEY_1, PUBLIC_KEY_2, PUBLIC_KEY_HEX_1};
use tw_any_coin::test_utils::address_utils::{
    test_address_derive, test_address_get_data, test_address_invalid, test_address_normalization,
    test_address_ss58_is_invalid, test_address_ss58_is_valid, test_address_valid, KeyType,
};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_polymesh_address_derive() {
    test_address_derive(
        CoinType::Polymesh,
        KeyType::PrivateKey(PRIVATE_KEY_1),
        PUBLIC_KEY_1,
    );
}

#[test]
fn test_polymesh_address_normalization() {
    test_address_normalization(CoinType::Polymesh, PUBLIC_KEY_1, PUBLIC_KEY_1);
}

#[test]
fn test_polymesh_address_is_valid() {
    // Polymesh
    test_address_ss58_is_valid(
        CoinType::Polymesh,
        "2DxwekgWwK7sqVeuXGmaXLZUvwnewLTs2rvU2CFKLgvvYwCG",
        12, // Polymesh ss58
    );
    test_address_valid(CoinType::Polymesh, PUBLIC_KEY_1);
    test_address_valid(CoinType::Polymesh, PUBLIC_KEY_2);
}

#[test]
fn test_polymesh_address_invalid() {
    // Substrate ed25519
    test_address_ss58_is_invalid(
        CoinType::Polymesh,
        "5FqqU2rytGPhcwQosKRtW1E3ha6BJKAjHgtcodh71dSyXhoZ",
        12, // Polymesh ss58
    );
    test_address_ss58_is_invalid(
        CoinType::Polymesh,
        "JCViCkwMdGWKpf7Wogb8EFtDmaYTEZGEg6ah4svUPGnnpc7A",
        12, // Polymesh ss58
    );
    test_address_invalid(
        CoinType::Polymesh,
        "5HUUBD9nsjaKKUVB3XV87CcjcEDu7sDH2G32NAj6uNqgWp9G",
    );
}

#[test]
fn test_polymesh_address_get_data() {
    test_address_get_data(CoinType::Polymesh, PUBLIC_KEY_1, PUBLIC_KEY_HEX_1);
}
