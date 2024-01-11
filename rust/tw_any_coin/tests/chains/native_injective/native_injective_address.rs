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
fn test_native_injective_address_normalization() {
    test_address_normalization(
        CoinType::NativeInjective,
        "inj14py36sx57ud82t9yrks9z6hdsrpn5x6k8tf7m3",
        "inj14py36sx57ud82t9yrks9z6hdsrpn5x6k8tf7m3",
    );
}

#[test]
fn test_native_injective_address_is_valid() {
    test_address_valid(
        CoinType::NativeInjective,
        "inj13u6g7vqgw074mgmf2ze2cadzvkz9snlwcrtq8a",
    );
    test_address_valid(
        CoinType::NativeInjective,
        "inj1xmpkmxr4as00em23tc2zgmuyy2gr4h3wgcl6vd",
    );
}

#[test]
fn test_native_injective_address_invalid() {
    test_address_invalid(
        CoinType::NativeInjective,
        "ini13u6g7vqgw074mgmf2ze2cadzvkz9snlwcrtq8a",
    );
}

#[test]
fn test_native_injective_address_get_data() {
    test_address_get_data(
        CoinType::NativeInjective,
        "inj1xmpkmxr4as00em23tc2zgmuyy2gr4h3wgcl6vd",
        "36c36d9875ec1efced515e14246f8422903ade2e",
    );
}

#[test]
fn test_any_address_is_valid_bech32() {
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Cosmos,
        address: "evmos14py36sx57ud82t9yrks9z6hdsrpn5x6k0r05np",
        hrp: "evmos",
    });
}

#[test]
fn test_any_address_create_bech32_with_public_key() {
    test_address_create_bech32_with_public_key(AddressCreateBech32WithPublicKey {
        coin: CoinType::NativeInjective,
        private_key: "8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed",
        public_key_type: PublicKeyType::Secp256k1Extended,
        hrp: "inj",
        expected: "inj14py36sx57ud82t9yrks9z6hdsrpn5x6k8tf7m3",
    });
}
