// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::address_utils::{
    test_address_base58_is_valid, test_address_bech32_is_valid,
    test_address_create_base58_with_public_key, test_address_create_bech32_with_public_key,
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
    AddressBase58IsValid, AddressBech32IsValid, AddressCreateBase58WithPublicKey,
    AddressCreateBech32WithPublicKey,
};
use tw_coin_registry::coin_type::CoinType;
use tw_keypair::tw::PublicKeyType;

#[test]
fn test_bitcoin_address_normalization() {
    test_address_normalization(
        CoinType::Bitcoin,
        "19cAJn4Ms8jodBBGtroBNNpCZiHAWGAq7X",
        "19cAJn4Ms8jodBBGtroBNNpCZiHAWGAq7X",
    );
}

#[test]
fn test_bitcoin_address_is_valid() {
    // P2PKH
    test_address_valid(CoinType::Bitcoin, "1MrZNGN7mfWZiZNQttrzHjfw72jnJC2JNx");
    // P2WSH address: Witness V0, 32 bytes program
    test_address_valid(
        CoinType::Bitcoin,
        "bc1qunq74p3h8425hr6wllevlvqqr6sezfxj262rff",
    );
    // P2TR address: Witness V1, 32 bytes program
    test_address_valid(
        CoinType::Bitcoin,
        "bc1pwse34zfpvt344rvlt7tw0ngjtfh9xasc4q03avf0lk74jzjpzjuqaz7ks5",
    );
}

#[test]
fn test_bitcoin_address_is_valid_base58() {
    // Litecoin P2PKH
    test_address_base58_is_valid(AddressBase58IsValid {
        coin: CoinType::Bitcoin,
        address: "LW6HjAU6GL9fK2LZWUA6VZCzomTdrpx3nr",
        p2pkh: 48,
        p2sh: 50,
    });
    // Dogecoin P2SH
    test_address_base58_is_valid(AddressBase58IsValid {
        coin: CoinType::Bitcoin,
        address: "AETZJzedcmLM2rxCM6VqCGF3YEMUjA3jMw",
        p2pkh: 30,
        p2sh: 22,
    });
}

#[test]
fn test_bitcoin_address_is_valid_bech32() {
    // Litecoin P2WPKH.
    test_address_bech32_is_valid(AddressBech32IsValid {
        coin: CoinType::Bitcoin,
        address: "ltc1q3m3ujh350qrqdl33pv7pjw0d0m9qnm6qjcjpga",
        hrp: "ltc",
    });
}

#[test]
fn test_bitcoin_address_invalid() {
    test_address_invalid(
        CoinType::Bitcoin,
        "0xb16db98b365b1f89191996942612b14f1da4bd5f",
    );
    test_address_invalid(CoinType::Bitcoin, "");
    // Litecoin P2PKH address is not valid without a prefix.
    test_address_invalid(CoinType::Bitcoin, "LW6HjAU6GL9fK2LZWUA6VZCzomTdrpx3nr");
    // Litecoin P2WPKH address is not valid without a prefix.
    test_address_invalid(
        CoinType::Bitcoin,
        "ltc1q3m3ujh350qrqdl33pv7pjw0d0m9qnm6qjcjpga",
    );
}

#[test]
fn test_bitcoin_address_legacy_get_data() {
    test_address_get_data(
        CoinType::Bitcoin,
        "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx",
        "769bdff96a02f9135a1d19b749db6a78fe07dc90",
    );
}

#[test]
fn test_bitcoin_address_segwit_get_data() {
    test_address_get_data(
        CoinType::Bitcoin,
        "bc1qunq74p3h8425hr6wllevlvqqr6sezfxj262rff",
        "e4c1ea86373d554b8f4efff2cfb0001ea19124d2",
    );
}

#[test]
fn test_bitcoin_address_taproot_get_data() {
    test_address_get_data(
        CoinType::Bitcoin,
        "bc1pwse34zfpvt344rvlt7tw0ngjtfh9xasc4q03avf0lk74jzjpzjuqaz7ks5",
        "74331a892162e35a8d9f5f96e7cd125a6e537618a81f1eb12ffdbd590a4114b8",
    );
}

#[test]
fn test_bitcoin_address_create_base58_with_public_key() {
    test_address_create_base58_with_public_key(AddressCreateBase58WithPublicKey {
        coin: CoinType::Bitcoin,
        public_key: "039d645d2ce630c2a9a6dbe0cbd0a8fcb7b70241cb8a48424f25593290af2494b9",
        public_key_type: PublicKeyType::Secp256k1,
        p2pkh: 0,
        p2sh: 5,
        expected: "12dNaXQtN5Asn2YFwT1cvciCrJa525fAe4",
    });

    test_address_create_base58_with_public_key(AddressCreateBase58WithPublicKey {
        coin: CoinType::Bitcoin,
        public_key: "03b49081a4d7ad24b20e209bc6fe10491aadb5607777baf0509a036cce96025db0",
        public_key_type: PublicKeyType::Secp256k1,
        p2pkh: 48,
        p2sh: 50,
        expected: "LW6HjAU6GL9fK2LZWUA6VZCzomTdrpx3nr",
    });
}

#[test]
fn test_bitcoin_address_create_bech32_with_public_key() {
    test_address_create_bech32_with_public_key(AddressCreateBech32WithPublicKey {
        coin: CoinType::Bitcoin,
        private_key: "55f9cbb0376c422946fa28397c1219933ac60b312ede41bfacaf701ecd546625",
        public_key_type: PublicKeyType::Secp256k1,
        hrp: "ltc",
        expected: "ltc1qytnqzjknvv03jwfgrsmzt0ycmwqgl0asjnaxwu",
    });
}
