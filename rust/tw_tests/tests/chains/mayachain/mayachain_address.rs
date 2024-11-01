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
fn test_mayachain_address_normalization() {
    test_address_normalization(
        CoinType::MAYAChain,
        "maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0wclun",
        "maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0wclun",
    );
}

#[test]
fn test_mayachain_address_is_valid() {
    test_address_valid(
        CoinType::MAYAChain,
        "maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0wclun",
    );
    test_address_valid(
        CoinType::MAYAChain,
        "maya1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2knf5vy",
    );
}

#[test]
fn test_mayachain_address_invalid() {
    test_address_invalid(
        CoinType::MAYAChain,
        "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
    );
    test_address_invalid(
        CoinType::MAYAChain,
        "mayar1z53wwe7md6cewz9sqwqzn0aavpaun0gw0e",
    );
}

#[test]
fn test_mayachain_address_get_data() {
    test_address_get_data(
        CoinType::MAYAChain,
        "maya1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2knf5vy",
        "c1e4df75a50e1d5b45d6e03f6157bc6169bfb46a",
    );
}

#[test]
fn test_any_address_is_valid_bech32() {
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Cosmos,
        address: "maya1c8jd7ad9pcw4k3wkuqlkz4auv95mldr2knf5vy",
        hrp: "maya",
    });
}

#[test]
fn test_any_address_create_bech32_with_public_key() {
    test_address_create_bech32_with_public_key(AddressCreateBech32WithPublicKey {
        coin: CoinType::Cosmos,
        private_key: "8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed",
        public_key_type: PublicKeyType::Secp256k1,
        hrp: "maya",
        expected: "maya1p05ufmhfpkjzqmc2u8humvgcqatq0esjq4a52m",
    });
}
