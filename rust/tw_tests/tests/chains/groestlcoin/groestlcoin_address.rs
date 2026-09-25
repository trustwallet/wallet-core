// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_create_base58_with_public_key, test_address_derive,
    test_address_derive_with_derivation, test_address_get_data, test_address_invalid,
    test_address_normalization, test_address_valid, AddressCreateBase58WithPublicKey, KeyType,
};
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::tw_derivation::TWDerivation;
use tw_keypair::tw::PublicKeyType;

#[test]
fn test_groestlcoin_address_derive() {
    test_address_derive(
        CoinType::Groestlcoin,
        KeyType::PrivateKey("dc334e7347f2f9f72fce789b11832bdf78adf0158bc6617e6d2d2a530a0d4bc6"),
        "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne",
    );
    test_address_derive_with_derivation(
        CoinType::Groestlcoin,
        KeyType::PublicKey("03b85cc59b67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91"),
        "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM",
        TWDerivation::BitcoinLegacy,
    );
}

#[test]
fn test_groestlcoin_address_normalization() {
    test_address_normalization(
        CoinType::Groestlcoin,
        "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM",
        "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM",
    );
}

#[test]
fn test_groestlcoin_address_is_valid() {
    test_address_valid(CoinType::Groestlcoin, "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM");
    test_address_valid(
        CoinType::Groestlcoin,
        "grs1qnzhs4t9r3zn7zqj02pwqxd3xmyyw8d22q55nf8",
    );
    test_address_valid(
        CoinType::Groestlcoin,
        "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne",
    );
    test_address_valid(CoinType::Groestlcoin, "31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P");
}

#[test]
fn test_groestlcoin_address_invalid() {
    // Invalid checksum.
    test_address_invalid(CoinType::Groestlcoin, "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLo");
    // HEX address.
    test_address_invalid(
        CoinType::Groestlcoin,
        "98af0aaca388a7e1024f505c033626d908e3b5",
    );
    // Bitcoin address.
    test_address_invalid(CoinType::Groestlcoin, "1JAd7XCBzGudGpJQSDSfpmJhiygtLQWaGL");
    // Bitcoin Segwit address.
    test_address_invalid(
        CoinType::Groestlcoin,
        "bc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z00ppggv",
    );
    // sha256d checksum instead of groestl512d
    test_address_invalid(CoinType::Groestlcoin, "Fj62rBJi8LvbmWu2jzkaUX1NFXLEsNpjgw");
    // valid checksum, but testnet prefix
    test_address_invalid(CoinType::Groestlcoin, "mvbu1Gdy8SUjTenqerxUaZyYjmvedc787y");
    // sha256d checksum instead of groestl512d
    test_address_invalid(CoinType::Groestlcoin, "31inaRqambLsd9D7Ke4USZmGEVd3LVt8yd");
    // valid checksum, but testnet prefix
    test_address_invalid(CoinType::Groestlcoin, "2N4Q5FhU2497BryFfUgbqkAJE87aKDv3V3e");
}

#[test]
fn test_groestlcoin_address_get_data() {
    test_address_get_data(
        CoinType::Groestlcoin,
        "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM",
        "98af0aaca388a7e1024f505c033626d908e3b54a",
    );
    test_address_get_data(
        CoinType::Groestlcoin,
        "grs1qnzhs4t9r3zn7zqj02pwqxd3xmyyw8d22q55nf8",
        "98af0aaca388a7e1024f505c033626d908e3b54a",
    );
}

#[test]
fn test_groestlcoin_address_create_base58_with_public_key() {
    test_address_create_base58_with_public_key(AddressCreateBase58WithPublicKey {
        coin: CoinType::Groestlcoin,
        public_key: "03b85cc59b67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91",
        public_key_type: PublicKeyType::Secp256k1,
        p2pkh: 36,
        p2sh: 5,
        expected: "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM",
    });
}
