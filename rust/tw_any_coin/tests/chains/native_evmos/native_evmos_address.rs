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
fn test_native_evmos_address_normalization() {
    test_address_normalization(
        CoinType::NativeEvmos,
        "evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw34",
        "evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw34",
    );
}

#[test]
fn test_native_evmos_address_is_valid() {
    test_address_valid(
        CoinType::NativeEvmos,
        "evmos14py36sx57ud82t9yrks9z6hdsrpn5x6k0r05np",
    );
    test_address_valid(
        CoinType::NativeEvmos,
        "evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw34",
    );
}

#[test]
fn test_native_evmos_address_invalid() {
    test_address_invalid(
        CoinType::NativeEvmos,
        "evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw",
    );
    // Cosmos has a different `hrp`.
    test_address_invalid(
        CoinType::Cosmos,
        "evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw34",
    );
}

#[test]
fn test_native_evmos_address_get_data() {
    test_address_get_data(
        CoinType::NativeEvmos,
        "evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw34",
        "f1829676db577682e944fc3493d451b67ff3e29f",
    );
}

#[test]
fn test_any_address_is_valid_bech32() {
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Cosmos,
        address: "evmos14py36sx57ud82t9yrks9z6hdsrpn5x6k0r05np",
        hrp: "evmos",
    });
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::NativeEvmos,
        address: "evmos14py36sx57ud82t9yrks9z6hdsrpn5x6k0r05np",
        hrp: "evmos",
    });
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::NativeEvmos,
        address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
        hrp: "cosmos",
    });
}

#[test]
fn test_any_address_create_bech32_with_public_key() {
    test_address_create_bech32_with_public_key(AddressCreateBech32WithPublicKey {
        coin: CoinType::NativeEvmos,
        private_key: "8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed",
        public_key_type: PublicKeyType::Secp256k1Extended,
        hrp: "evmos",
        expected: "evmos14py36sx57ud82t9yrks9z6hdsrpn5x6k0r05np",
    });
}
