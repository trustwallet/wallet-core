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
fn test_{COIN_ID}_address_normalization() {
    test_address_normalization(CoinType::{COIN_TYPE}, "DENORMALIZED", "EXPECTED");
}

#[test]
fn test_{COIN_ID}_address_is_valid() {
    test_address_valid(CoinType::{COIN_TYPE}, "VALID {COIN_TYPE} ADDRESS");
}

#[test]
fn test_{COIN_ID}_address_invalid() {
    test_address_invalid(CoinType::{COIN_TYPE}, "INVALID ADDRESS");
    // Cosmos has a different `hrp`.
    test_address_invalid(CoinType::Cosmos, "VALID {COIN_TYPE} ADDRESS");
}

#[test]
fn test_{COIN_ID}_address_get_data() {
    test_address_get_data(CoinType::{COIN_TYPE}, "ADDRESS", "HEX(DATA)");
}

#[test]
fn test_{COIN_ID}_is_valid_bech32() {
    // {COIN_TYPE} address must be valid if its Base32 prefix passed.
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::{COIN_TYPE},
        address: "{COIN_TYPE} ADDRESS",
        hrp: "{HRP}",
    });
    // {COIN_TYPE} address must be valid for the standard Cosmos hub if its Base32 prefix passed.
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Cosmos,
        address: "{COIN_TYPE} ADDRESS",
        hrp: "{HRP}",
    });
    // Cosmos address must be valid with "cosmos" hrp.
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::{COIN_TYPE},
        address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
        hrp: "cosmos",
    });
}

#[test]
fn test_any_address_create_bech32_with_public_key() {
    test_address_create_bech32_with_public_key(AddressCreateBech32WithPublicKey {
        // TODO consider using `CoinType::{COIN_TYPE}` if the chain's `addressHasher` is different from Cosmos's.
        coin: CoinType::Cosmos,
        private_key: "PRIVATE_KEY",
        // TODO consider using another `PublicKeyType` if the chain's `publicKeyType` is different from Cosmos's.
        public_key_type: PublicKeyType::Secp256k1,
        hrp: "{HRP}",
        expected: "{COIN_TYPE} ADDRESS",
    });
}

