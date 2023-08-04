// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::borrow::Cow;
use tw_coin_entry::error::SigningErrorType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_evm::evm_entry::EvmEntryExt;
use tw_number::U256;
use tw_proto::EthereumRlp::Proto as RlpProto;
use tw_proto::{deserialize, serialize};
use tw_ronin::entry::RoninEntry;
use RlpProto::mod_RlpItem::OneOfitem as Item;

fn make_item(item: Item) -> RlpProto::RlpItem {
    RlpProto::RlpItem { item }
}

#[track_caller]
fn test_encode(item: Item, expected: &str) {
    let input = RlpProto::EncodingInput {
        item: Some(make_item(item)),
    };
    let input_data = serialize(&input).unwrap();
    let output_data = RoninEntry.encode_rlp(&input_data).unwrap();
    let output: RlpProto::EncodingOutput =
        deserialize(&output_data).expect("Coin entry returned an invalid output");
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(output.encoded.to_hex(), expected);
}

#[test]
fn test_rlp_encode_ronin_address() {
    let ronin_addr = "ronin:6b175474e89094c44da98b954eedeac495271d0f";
    test_encode(
        Item::address(Cow::from(ronin_addr)),
        "946b175474e89094c44da98b954eedeac495271d0f",
    );
}

#[test]
fn test_rlp_encode_ronin_eip1559() {
    let chain_id = U256::encode_be_compact(10);
    let nonce = U256::encode_be_compact(6);
    let max_inclusion_fee_per_gas = 2_000_000_000;
    let max_fee_per_gas = U256::encode_be_compact(3_000_000_000);
    let gas_limit = U256::encode_be_compact(21_100);
    let to = "ronin:6b175474e89094c44da98b954eedeac495271d0f";
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
