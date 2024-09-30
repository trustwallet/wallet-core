// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::rlp_encoder::{RlpEncoder, RECURSION_LIMIT};
use tw_number::U256;
use tw_proto::EthereumRlp::Proto as RlpProto;
use RlpProto::mod_RlpItem::OneOfitem as Item;

fn make_item(item: Item) -> RlpProto::RlpItem {
    RlpProto::RlpItem { item }
}

#[track_caller]
fn test_encode(item: Item, expected: &str) {
    let input = RlpProto::EncodingInput {
        item: Some(make_item(item)),
    };
    let output = RlpEncoder::<StandardEvmContext>::encode_with_proto(input);
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(output.encoded.to_hex(), expected);
}

#[test]
fn test_encode_string() {
    test_encode(Item::string_item(Cow::from("")), "80");
    test_encode(Item::string_item(Cow::from("d")), "64");
    test_encode(Item::string_item(Cow::from("dog")), "83646f67");
}

#[test]
fn test_encode_number_u64() {
    test_encode(Item::number_u64(0), "80");
    test_encode(Item::number_u64(127), "7f");
    test_encode(Item::number_u64(128), "8180");
    test_encode(Item::number_u64(255), "81ff");
    test_encode(Item::number_u64(256), "820100");
    test_encode(Item::number_u64(1024), "820400");
    test_encode(Item::number_u64(0xffff), "82ffff");
    test_encode(Item::number_u64(0x010000), "83010000");
    test_encode(Item::number_u64(0xffffff), "83ffffff");
    test_encode(Item::number_u64(0xffffffff), "84ffffffff");
    test_encode(Item::number_u64(0xffffffffffffff), "87ffffffffffffff");
}

#[test]
fn test_ethereum_rlp_number_u256() {
    macro_rules! test_encode_u256 {
        ($num_str:literal => $expected:literal) => {
            let num = U256::from_str($num_str).unwrap().to_big_endian_compact();
            test_encode(Item::number_u256(Cow::from(num)), $expected);
        };
    }

    test_encode_u256!("0" => "80");
    test_encode_u256!("1" => "01");
    test_encode_u256!("127" => "7f");
    test_encode_u256!("128" => "8180");
    test_encode_u256!("256" => "820100");
    test_encode_u256!("1024" => "820400");

    // 0xffffff
    test_encode_u256!("16777215" => "83ffffff");
    // 0xffffffff
    test_encode_u256!("4294967295" => "84ffffffff");
    // 0xffffffffffffff
    test_encode_u256!("72057594037927935" => "87ffffffffffffff");
    // 0x102030405060708090a0b0c0d0e0f2
    test_encode_u256!("83729609699884896815286331701780722" => "8f102030405060708090a0b0c0d0e0f2");
    // 0x0100020003000400050006000700080009000a000b000c000d000e01
    test_encode_u256!("105315505618206987246253880190783558935785933862974822347068935681" => "9c0100020003000400050006000700080009000a000b000c000d000e01");
    // 0x0100000000000000000000000000000000000000000000000000000000000000
    test_encode_u256!("452312848583266388373324160190187140051835877600158453279131187530910662656" => "a00100000000000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_ethereum_rlp_raw_encoded() {
    let raw_encoded = "946b175474e89094c44da98b954eedeac495271d0f"
        .decode_hex()
        .unwrap();
    test_encode(
        Item::raw_encoded(Cow::from(raw_encoded)),
        "946b175474e89094c44da98b954eedeac495271d0f",
    );

    let empty: &[u8] = &[];
    test_encode(Item::raw_encoded(Cow::from(empty)), "");
}

#[test]
fn test_ethereum_rlp_list() {
    // Empty list.
    let list = RlpProto::RlpList {
        items: Vec::default(),
    };
    test_encode(Item::list(list), "c0");

    // [1, 2, 3] list.
    let list = RlpProto::RlpList {
        items: vec![
            make_item(Item::number_u64(1)),
            make_item(Item::number_u64(2)),
            make_item(Item::number_u64(3)),
        ],
    };
    test_encode(Item::list(list), "c3010203");

    // ["a", "b"] list.
    let list = RlpProto::RlpList {
        items: vec![
            make_item(Item::string_item(Cow::from("a"))),
            make_item(Item::string_item(Cow::from("b"))),
        ],
    };
    test_encode(Item::list(list), "c26162");

    // ["cat", "dog"] list.
    let list = RlpProto::RlpList {
        items: vec![
            make_item(Item::string_item(Cow::from("cat"))),
            make_item(Item::string_item(Cow::from("dog"))),
        ],
    };
    test_encode(Item::list(list), "c88363617483646f67");

    // ["cat", "dog"] list.
    let list = RlpProto::RlpList {
        items: vec![
            make_item(Item::string_item(Cow::from("cat"))),
            make_item(Item::string_item(Cow::from("dog"))),
        ],
    };
    test_encode(Item::list(list), "c88363617483646f67");
}

#[test]
fn test_ethereum_rlp_nested_list() {
    let l11 = RlpProto::RlpList {
        items: vec![
            make_item(Item::number_u64(1)),
            make_item(Item::number_u64(2)),
            make_item(Item::number_u64(3)),
        ],
    };
    let l12 = RlpProto::RlpList {
        items: vec![
            make_item(Item::string_item(Cow::from("apple"))),
            make_item(Item::string_item(Cow::from("banana"))),
            make_item(Item::string_item(Cow::from("cherry"))),
        ],
    };
    let l21 = RlpProto::RlpList {
        items: vec![
            make_item(Item::data(Cow::from("abcdef".decode_hex().unwrap()))),
            make_item(Item::data(Cow::from(
                "00010203040506070809".decode_hex().unwrap(),
            ))),
        ],
    };
    let l22 = RlpProto::RlpList {
        items: vec![
            make_item(Item::string_item(Cow::from("bitcoin"))),
            make_item(Item::string_item(Cow::from("beeenbee"))),
            make_item(Item::string_item(Cow::from("eth"))),
        ],
    };
    let l1 = RlpProto::RlpList {
        items: vec![make_item(Item::list(l11)), make_item(Item::list(l12))],
    };
    let l2 = RlpProto::RlpList {
        items: vec![make_item(Item::list(l21)), make_item(Item::list(l22))],
    };
    let list = RlpProto::RlpList {
        items: vec![make_item(Item::list(l1)), make_item(Item::list(l2))],
    };

    // The value is checked by using https://codechain-io.github.io/rlp-debugger/
    test_encode(
        Item::list(list),
        "f841d9c3010203d4856170706c658662616e616e6186636865727279e6cf83abcdef8a00010203040506070809d587626974636f696e88626565656e62656583657468"
    );
}

#[test]
fn test_ethereum_rlp_nested_list_recursion_limit() {
    fn make_nested_list(nested_lists_to_create: usize) -> RlpProto::RlpList<'static> {
        if nested_lists_to_create == 1 {
            // This is the last call in the recursion.
            return RlpProto::RlpList { items: Vec::new() };
        }

        let nested_list = make_nested_list(nested_lists_to_create - 1);
        RlpProto::RlpList {
            items: vec![make_item(Item::list(nested_list))],
        }
    }

    let nested_list = make_nested_list(RECURSION_LIMIT + 10);
    let input = RlpProto::EncodingInput {
        item: Some(make_item(Item::list(nested_list))),
    };

    let output = RlpEncoder::<StandardEvmContext>::encode_with_proto(input);
    assert_eq!(output.error, SigningErrorType::Error_invalid_params);
}

#[test]
fn test_ethereum_rlp_list_eip1559() {
    let chain_id = U256::encode_be_compact(10);
    let nonce = U256::encode_be_compact(6);
    let max_inclusion_fee_per_gas = 2_000_000_000;
    let max_fee_per_gas = U256::encode_be_compact(3_000_000_000);
    let gas_limit = U256::encode_be_compact(21_100);
    let to = "0x6b175474e89094c44da98b954eedeac495271d0f";
    let amount = 0;
    let payload = "a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1".decode_hex().unwrap();

    // Empty nested list.
    let access_list = RlpProto::RlpList {
        items: Vec::default(),
    };
    let list = RlpProto::RlpList {
        items: vec![
            make_item(Item::number_u256(chain_id)),
            make_item(Item::number_u256(nonce)),
            make_item(Item::number_u64(max_inclusion_fee_per_gas)),
            make_item(Item::number_u256(max_fee_per_gas)),
            make_item(Item::number_u256(gas_limit)),
            make_item(Item::address(Cow::from(to))),
            make_item(Item::number_u64(amount)),
            make_item(Item::data(Cow::from(payload))),
            make_item(Item::list(access_list)),
        ],
    };
    test_encode(
        Item::list(list),
        "f86c0a06847735940084b2d05e0082526c946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000000001ee0c29f50cb1c0"
    );
}
