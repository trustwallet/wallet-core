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
fn test_dydx_address_normalization() {
    test_address_normalization(
        CoinType::Dydx,
        "dydx1adl7usw7z2dnysyn7wvrghu0u0q6gr7jqs4gtt",
        "dydx1adl7usw7z2dnysyn7wvrghu0u0q6gr7jqs4gtt",
    );
}

#[test]
fn test_dydx_address_is_valid() {
    test_address_valid(
        CoinType::Dydx,
        "dydxvaloper1gf9yvztyvnc3aqrkjd8hfnaf8pk56sl7mzfage",
    );
}

#[test]
fn test_dydx_address_invalid() {
    test_address_invalid(
        CoinType::Dydx,
        "dydxvaloper1gf9yvztyvnc3aqrkjd8hfnaf8pk56sl7mz",
    );
    // Cosmos has a different `hrp`.
    test_address_invalid(
        CoinType::Cosmos,
        "dydx1adl7usw7z2dnysyn7wvrghu0u0q6gr7jqs4gtt",
    );
}

#[test]
fn test_dydx_address_get_data() {
    test_address_get_data(
        CoinType::Dydx,
        "dydxvaloper1gf9yvztyvnc3aqrkjd8hfnaf8pk56sl7mzfage",
        "424a46096464f11e8076934f74cfa9386d4d43fe",
    );
}

#[test]
fn test_dydx_is_valid_bech32() {
    // Dydx address must be valid if its Base32 prefix passed.
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Dydx,
        address: "dydx1adl7usw7z2dnysyn7wvrghu0u0q6gr7jqs4gtt",
        hrp: "dydx",
    });
    // Dydx address must be valid for the standard Cosmos hub if its Base32 prefix passed.
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Cosmos,
        address: "dydx1adl7usw7z2dnysyn7wvrghu0u0q6gr7jqs4gtt",
        hrp: "dydx",
    });
    // Cosmos address must be valid with "cosmos" hrp.
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Dydx,
        address: "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
        hrp: "cosmos",
    });
}

#[test]
fn test_any_address_create_bech32_with_public_key() {
    test_address_create_bech32_with_public_key(AddressCreateBech32WithPublicKey {
        coin: CoinType::Cosmos,
        private_key: "a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433",
        public_key_type: PublicKeyType::Secp256k1,
        hrp: "dydx",
        expected: "dydx1mry47pkga5tdswtluy0m8teslpalkdq0hc72uz",
    });
}
