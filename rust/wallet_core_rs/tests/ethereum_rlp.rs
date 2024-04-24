// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::ToHex;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::EthereumRlp::Proto as RlpProto;
use tw_proto::{deserialize, serialize};
use wallet_core_rs::ffi::ethereum::rlp::tw_ethereum_rlp_encode;
use RlpProto::mod_RlpItem::OneOfitem as Item;

#[test]
fn test_ethereum_rlp() {
    let item = RlpProto::RlpItem {
        item: Item::number_u64(128),
    };
    let input = RlpProto::EncodingInput { item: Some(item) };
    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output_data = TWDataHelper::wrap(unsafe {
        tw_ethereum_rlp_encode(CoinType::Ethereum as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_ethereum_rlp_encode returned nullptr");
    let output: RlpProto::EncodingOutput =
        deserialize(&output_data).expect("!tw_ethereum_rlp_encode returned an invalid output");

    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    let expected_encoded = "8180";
    assert_eq!(output.encoded.to_hex(), expected_encoded);
}
