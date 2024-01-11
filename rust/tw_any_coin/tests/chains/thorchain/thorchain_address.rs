// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_bech32_is_valid, test_address_create_bech32_with_public_key,
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
    AddressBech32IsValid, AddressCreateBech32WithPublicKey,
};
use tw_coin_registry::coin_type::CoinType;
use tw_keypair::tw::PublicKeyType;

#[test]
fn test_thorchain_address_normalization() {
    test_address_normalization(
        CoinType::THORChain,
        "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
        "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
    );
}

#[test]
fn test_thorchain_address_is_valid() {
    test_address_valid(
        CoinType::THORChain,
        "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
    );
    test_address_valid(
        CoinType::THORChain,
        "thor1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2kyhc65",
    );
}

#[test]
fn test_thorchain_address_invalid() {
    test_address_invalid(
        CoinType::THORChain,
        "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
    );
    test_address_invalid(
        CoinType::THORChain,
        "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0e",
    );
}

#[test]
fn test_thorchain_address_get_data() {
    test_address_get_data(
        CoinType::THORChain,
        "thor1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2kyhc65",
        "c1e4df75a50e1d5b45d6e03f6157bc6169bfb46a",
    );
}

#[test]
fn test_any_address_is_valid_bech32() {
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Cosmos,
        address: "thor1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2kyhc65",
        hrp: "thor",
    });
}

#[test]
fn test_any_address_create_bech32_with_public_key() {
    test_address_create_bech32_with_public_key(AddressCreateBech32WithPublicKey {
        coin: CoinType::Cosmos,
        private_key: "8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed",
        public_key_type: PublicKeyType::Secp256k1,
        hrp: "thor",
        expected: "thor1p05ufmhfpkjzqmc2u8humvgcqatq0esjqzrcut",
    });
}
