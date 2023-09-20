// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde_json::Value as Json;
use tw_coin_entry::error::SigningErrorType;
use tw_encoding::hex::DecodeHex;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::EthereumAbi::Proto as AbiProto;
use tw_proto::{deserialize, serialize};
use wallet_core_rs::ffi::ethereum::abi::tw_ethereum_abi_decode_contract_call;

const ETHEREUM_COIN_TYPE: u32 = 60;

#[test]
fn test_ethereum_rlp() {
    const CUSTOM_ABI_JSON: &str = include_str!("data/custom.json");
    const CUSTOM_DECODED_JSON: &str = include_str!("data/custom_decoded.json");

    let encoded = "ec37a4a000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000006400000000000000000000000000000000000000000000000000000000000000067472757374790000000000000000000000000000000000000000000000000000".decode_hex().unwrap();

    let input = AbiProto::ContractCallDecodingInput {
        encoded: encoded.into(),
        smart_contract_abi_json: CUSTOM_ABI_JSON.into(),
    };
    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output_data = TWDataHelper::wrap(unsafe {
        tw_ethereum_abi_decode_contract_call(ETHEREUM_COIN_TYPE, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_ethereum_abi_decode_contract_call returned nullptr");
    let output: AbiProto::ContractCallDecodingOutput = deserialize(&output_data)
        .expect("!tw_ethereum_abi_decode_contract_call returned an invalid output");

    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    let actual: Json = serde_json::from_str(&output.decoded_json).unwrap();
    let expected: Json = serde_json::from_str(CUSTOM_DECODED_JSON).unwrap();
    assert_eq!(actual, expected);
}
