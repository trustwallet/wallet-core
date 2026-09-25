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
fn test_zetachain_address_normalization() {
    test_address_normalization(
        CoinType::NativeZetaChain,
        "zeta14py36sx57ud82t9yrks9z6hdsrpn5x6kmxs0ne",
        "zeta14py36sx57ud82t9yrks9z6hdsrpn5x6kmxs0ne",
    );
}

#[test]
fn test_zetachain_address_is_valid() {
    test_address_valid(
        CoinType::NativeZetaChain,
        "zeta1em87eul072u3yz4vpgm9mdfszhu0clxekuw08f",
    );
    test_address_valid(
        CoinType::NativeZetaChain,
        "zeta17xpfvakm2amg962yls6f84z3kell8c5lxad43d",
    );
}

#[test]
fn test_zetachain_address_invalid() {
    test_address_invalid(
        CoinType::NativeZetaChain,
        "zeta17xpfvakm2amg962yls6f84z3kell8c5lxad4",
    );
    // Cosmos has a different `hrp`.
    test_address_invalid(
        CoinType::Cosmos,
        "zeta17xpfvakm2amg962yls6f84z3kell8c5lxad43d",
    );
}

#[test]
fn test_zetachain_address_get_data() {
    test_address_get_data(
        CoinType::NativeZetaChain,
        "zeta14py36sx57ud82t9yrks9z6hdsrpn5x6kmxs0ne",
        "A8491D40D4F71A752CA41DA0516AED80C33A1B56",
    );
}

#[test]
fn test_zetachain_is_valid_bech32() {
    // NativeZetaChain address must be valid if its Base32 prefix passed.
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::NativeZetaChain,
        address: "zeta14py36sx57ud82t9yrks9z6hdsrpn5x6kmxs0ne",
        hrp: "zeta",
    });
    // NativeZetaChain address must be valid for the standard Cosmos hub if its Base32 prefix passed.
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Cosmos,
        address: "zeta14py36sx57ud82t9yrks9z6hdsrpn5x6kmxs0ne",
        hrp: "zeta",
    });
    // Cosmos address must be valid with "cosmos" hrp.
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::NativeZetaChain,
        address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
        hrp: "cosmos",
    });
}

#[test]
fn test_any_address_create_bech32_with_public_key() {
    test_address_create_bech32_with_public_key(AddressCreateBech32WithPublicKey {
        coin: CoinType::NativeZetaChain,
        private_key: "8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed",
        public_key_type: PublicKeyType::Secp256k1Extended,
        hrp: "zeta",
        expected: "zeta14py36sx57ud82t9yrks9z6hdsrpn5x6kmxs0ne",
    });
}
