// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_any_coin::ffi::tw_any_address::{
    tw_any_address_create_with_public_key_derivation, tw_any_address_create_with_string,
    tw_any_address_data, tw_any_address_description, tw_any_address_is_valid,
};
use tw_any_coin::test_utils::TWAnyAddressHelper;
use tw_coin_entry::derivation::Derivation;
use tw_coin_registry::blockchain_type::BlockchainType;
use tw_coin_registry::registry::supported_coin_items;
use tw_encoding::hex::DecodeHex;
use tw_keypair::ffi::privkey::tw_private_key_get_public_key_by_type;
use tw_keypair::test_utils::tw_private_key_helper::TWPrivateKeyHelper;
use tw_keypair::test_utils::tw_public_key_helper::TWPublicKeyHelper;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;

const ETHEREUM_COIN_TYPE: u32 = 60;

#[test]
fn test_any_address_derive() {
    let private_key = TWPrivateKeyHelper::with_hex(
        "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
    );

    for coin in supported_coin_items() {
        let public_key = TWPublicKeyHelper::wrap(unsafe {
            tw_private_key_get_public_key_by_type(private_key.ptr(), coin.public_key_type as u32)
        });

        // TODO match `CoinType` when it's generated.
        let expected_address = match coin.blockchain {
            // By default, Bitcoin will return a P2PKH address.
            BlockchainType::Bitcoin => "19cAJn4Ms8jodBBGtroBNNpCZiHAWGAq7X",
            BlockchainType::Ethereum => "0xAc1ec44E4f0ca7D172B7803f6836De87Fb72b309",
            BlockchainType::Ronin => "ronin:Ac1ec44E4f0ca7D172B7803f6836De87Fb72b309",
            BlockchainType::InternetComputer => {
                "290cc7c359f44c8516fc169c5ed4f0f3ae2e24bf5de0d4c51f5e7545b5474faa"
            },
            BlockchainType::Unsupported => unreachable!(),
        };

        let any_address = TWAnyAddressHelper::wrap(unsafe {
            tw_any_address_create_with_public_key_derivation(
                public_key.ptr(),
                coin.coin_id,
                Derivation::Default as u32,
            )
        });

        let description =
            TWStringHelper::wrap(unsafe { tw_any_address_description(any_address.ptr()) });
        assert_eq!(description.to_string(), Some(expected_address.to_string()));
    }
}

#[test]
fn test_any_address_normalize_eth() {
    for coin in supported_coin_items() {
        let (denormalized, expected_normalized) = match coin.blockchain {
            BlockchainType::Bitcoin => (
                "19cAJn4Ms8jodBBGtroBNNpCZiHAWGAq7X",
                "19cAJn4Ms8jodBBGtroBNNpCZiHAWGAq7X",
            ),
            BlockchainType::Ethereum => (
                "0xb16db98b365b1f89191996942612b14f1da4bd5f",
                "0xb16Db98B365B1f89191996942612B14F1Da4Bd5f",
            ),
            BlockchainType::Ronin => (
                "0xb16db98b365b1f89191996942612b14f1da4bd5f",
                "ronin:b16Db98B365B1f89191996942612B14F1Da4Bd5f",
            ),
            BlockchainType::InternetComputer => (
                "290CC7C359F44C8516FC169C5ED4F0F3AE2E24BF5DE0D4C51F5E7545B5474FAA",
                "290cc7c359f44c8516fc169c5ed4f0f3ae2e24bf5de0d4c51f5e7545b5474faa",
            ),
            BlockchainType::Unsupported => unreachable!(),
        };

        let denormalized = TWStringHelper::create(denormalized);

        let any_address = TWAnyAddressHelper::wrap(unsafe {
            tw_any_address_create_with_string(denormalized.ptr(), coin.coin_id)
        });

        let normalized =
            TWStringHelper::wrap(unsafe { tw_any_address_description(any_address.ptr()) });

        assert_eq!(
            normalized.to_string(),
            Some(expected_normalized.to_string())
        );
    }
}

#[test]
fn test_any_address_is_valid_coin() {
    for coin in supported_coin_items() {
        let valid = match coin.blockchain {
            BlockchainType::Bitcoin => vec![
                "1MrZNGN7mfWZiZNQttrzHjfw72jnJC2JNx",
                "bc1qunq74p3h8425hr6wllevlvqqr6sezfxj262rff",
                "bc1pwse34zfpvt344rvlt7tw0ngjtfh9xasc4q03avf0lk74jzjpzjuqaz7ks5",
            ],
            BlockchainType::Ethereum => vec![
                "0xb16db98b365b1f89191996942612b14f1da4bd5f",
                "0xb16Db98B365B1f89191996942612B14F1Da4Bd5f",
            ],
            BlockchainType::Ronin => vec![
                "0xb16db98b365b1f89191996942612b14f1da4bd5f",
                "0xb16Db98B365B1f89191996942612B14F1Da4Bd5f",
                "ronin:b16db98b365b1f89191996942612b14f1da4bd5f",
                "ronin:b16Db98B365B1f89191996942612B14F1Da4Bd5f",
            ],
            BlockchainType::InternetComputer => vec![
                "fb257577279ecac604d4780214af95aa6adc3a814f6f3d6d7ac844d1deca500a",
                "e90c48d54847f4758f1d6b589a1db2500757a49a6722d4f775e050107b4b752d",
                "a7c5baf393aed527ef6fb3869fbf84dd4e562edf9b04bd8f9bfbd6b8e6a22776",
                "4cb2ca5cfcfa1d952f8cd7f0ec46c96e1023ab057b83a2c7ce236b9e71ccca0b",
            ],
            _ => unreachable!(),
        };

        for valid_addr in valid {
            let valid = TWStringHelper::create(valid_addr);
            assert!(unsafe { tw_any_address_is_valid(valid.ptr(), coin.coin_id) });
        }
    }
}

#[test]
fn test_any_address_is_valid_coin_invalid() {
    for coin in supported_coin_items() {
        let invalid = match coin.blockchain {
            BlockchainType::Bitcoin => {
                vec!["0xb16db98b365b1f89191996942612b14f1da4bd5f"]
            },
            BlockchainType::Ethereum | BlockchainType::Ronin => {
                vec!["b16Db98B365B1f89191996942612B14F1Da4Bd5f"]
            },
            BlockchainType::InternetComputer => vec![
                "3357cba483f268d044d4bbd4639f82c16028a76eebdf62c51bc11fc918d278b",
                "3357cba483f268d044d4bbd4639f82c16028a76eebdf62c51bc11fc918d278bce",
                "553357cba483f268d044d4bbd4639f82c16028a76eebdf62c51bc11fc918d278",
            ],
            BlockchainType::Unsupported => unreachable!(),
        };

        for invalid_addr in invalid {
            let valid = TWStringHelper::create(invalid_addr);
            assert!(!unsafe { tw_any_address_is_valid(valid.ptr(), coin.coin_id) });
        }
    }
}

#[test]
fn test_any_address_get_data_eth() {
    let addr = "0xb16Db98B365B1f89191996942612B14F1Da4Bd5f";

    let address_str = TWStringHelper::create(addr);
    let any_address = TWAnyAddressHelper::wrap(unsafe {
        tw_any_address_create_with_string(address_str.ptr(), ETHEREUM_COIN_TYPE)
    });
    let data = TWDataHelper::wrap(unsafe { tw_any_address_data(any_address.ptr()) });
    assert_eq!(data.to_vec(), Some(addr.decode_hex().unwrap()));
}
