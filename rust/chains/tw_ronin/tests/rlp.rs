// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_coin_entry::error::SigningErrorType;
use tw_encoding::hex::ToHex;
use tw_evm::evm_entry::EvmEntryExt;
use tw_proto::EthereumRlp::Proto as RlpProto;
use tw_proto::{deserialize, serialize};
use tw_ronin::entry::RoninEntry;
use RlpProto::mod_RlpItem::OneOfitem as Item;

#[test]
fn test_rlp_encode_ronin_address() {
    let ronin_addr = "ronin:6b175474e89094c44da98b954eedeac495271d0f";
    let input = RlpProto::EncodingInput {
        item: Some(RlpProto::RlpItem {
            item: Item::address(Cow::from(ronin_addr)),
        }),
    };
    let input_data = serialize(&input).unwrap();
    let output_data = RoninEntry.encode_rlp(&input_data).unwrap();
    let output: RlpProto::EncodingOutput =
        deserialize(&output_data).expect("Coin entry returned an invalid output");
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(
        output.encoded.to_hex(),
        "946b175474e89094c44da98b954eedeac495271d0f"
    );
}
