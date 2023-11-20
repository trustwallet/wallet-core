// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_any_coin::test_utils::address_utils::{
    test_address_get_data, test_address_invalid, test_address_normalization, test_address_valid,
};
use tw_coin_registry::blockchain_type::BlockchainType;
use tw_coin_registry::registry::coin_items_by_blockchain;

#[test]
fn test_ethereum_address_normalization() {
    for coin in coin_items_by_blockchain(BlockchainType::Ethereum) {
        test_address_normalization(
            coin.coin_id,
            "0xb16db98b365b1f89191996942612b14f1da4bd5f",
            "0xb16Db98B365B1f89191996942612B14F1Da4Bd5f",
        );
    }
}

#[test]
fn test_ethereum_address_is_valid() {
    for coin in coin_items_by_blockchain(BlockchainType::Ethereum) {
        test_address_valid(coin.coin_id, "0xb16db98b365b1f89191996942612b14f1da4bd5f");
        test_address_valid(coin.coin_id, "0xb16Db98B365B1f89191996942612B14F1Da4Bd5f");
    }
}

#[test]
fn test_ethereum_address_invalid() {
    for coin in coin_items_by_blockchain(BlockchainType::Ethereum) {
        test_address_invalid(coin.coin_id, "b16Db98B365B1f89191996942612B14F1Da4Bd5f");
    }
}

#[test]
fn test_ethereum_address_get_data() {
    for coin in coin_items_by_blockchain(BlockchainType::Ethereum) {
        test_address_get_data(
            coin.coin_id,
            "0xb16Db98B365B1f89191996942612B14F1Da4Bd5f",
            "b16Db98B365B1f89191996942612B14F1Da4Bd5f",
        );
    }
}
