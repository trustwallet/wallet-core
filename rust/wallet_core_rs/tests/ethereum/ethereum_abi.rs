// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde_json::{json, Value as Json};
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use tw_number::U256;
use tw_proto::EthereumAbi::{Proto as AbiProto, Proto};
use tw_proto::{deserialize, serialize};
use wallet_core_rs::ffi::ethereum::abi::{
    tw_ethereum_abi_decode_contract_call, tw_ethereum_abi_decode_params,
    tw_ethereum_abi_decode_value, tw_ethereum_abi_encode_function,
    tw_ethereum_abi_function_get_signature,
};

use tw_coin_registry::coin_type::CoinType;
use Proto::mod_ParamType::OneOfparam as ParamTypeEnum;
use Proto::mod_Token::OneOftoken as TokenEnum;
use Proto::AbiError as AbiErrorKind;

fn param(name: &str, kind: ParamTypeEnum<'static>) -> Proto::Param<'static> {
    Proto::Param {
        name: name.to_string().into(),
        param: Some(Proto::ParamType { param: kind }),
    }
}

fn named_token(name: &str, token: TokenEnum<'static>) -> Proto::Token<'static> {
    Proto::Token {
        name: name.to_string().into(),
        token,
    }
}

fn number_n<const BITS: u32>(value: u64) -> Proto::NumberNParam<'static> {
    Proto::NumberNParam {
        bits: BITS,
        value: U256::encode_be_compact(value),
    }
}

#[test]
fn test_ethereum_abi_decode_contract_call() {
    const CUSTOM_ABI_JSON: &str = include_str!("data/custom.json");
    const CUSTOM_DECODED_JSON: &str = include_str!("data/custom_decoded.json");

    let encoded = "ec37a4a000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000006400000000000000000000000000000000000000000000000000000000000000067472757374790000000000000000000000000000000000000000000000000000".decode_hex().unwrap();

    let input = AbiProto::ContractCallDecodingInput {
        encoded: encoded.into(),
        smart_contract_abi_json: CUSTOM_ABI_JSON.into(),
    };
    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output_data = TWDataHelper::wrap(unsafe {
        tw_ethereum_abi_decode_contract_call(CoinType::Ethereum as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_ethereum_abi_decode_contract_call returned nullptr");
    let output: AbiProto::ContractCallDecodingOutput = deserialize(&output_data)
        .expect("!tw_ethereum_abi_decode_contract_call returned an invalid output");

    assert_eq!(output.error, AbiErrorKind::OK);
    assert!(output.error_message.is_empty());

    let actual: Json = serde_json::from_str(&output.decoded_json).unwrap();
    let expected: Json = serde_json::from_str(CUSTOM_DECODED_JSON).unwrap();
    assert_eq!(actual, expected);
}

#[test]
fn test_ethereum_abi_decode_params() {
    let abi_json = json!([
        {
            "internalType": "address",
            "name": "to",
            "type": "address"
        },
        {
            "internalType": "bool",
            "name": "approved",
            "type": "bool"
        }
    ]);
    let abi_json = serde_json::to_string(&abi_json).unwrap();
    // https://etherscan.io/tx/0xc2744000a107aee4761cf8a638657f91c3003a54e2f1818c37d781be7e48187a
    let encoded = "00000000000000000000000088341d1a8f672d2780c8dc725902aae72f143b0c0000000000000000000000000000000000000000000000000000000000000001".decode_hex().unwrap();

    let input = AbiProto::ParamsDecodingInput {
        encoded: encoded.into(),
        abi: AbiProto::mod_ParamsDecodingInput::OneOfabi::abi_json(abi_json.into()),
    };
    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output_data = TWDataHelper::wrap(unsafe {
        tw_ethereum_abi_decode_params(CoinType::Ethereum as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_ethereum_abi_decode_params returned nullptr");

    let output: AbiProto::ParamsDecodingOutput = deserialize(&output_data)
        .expect("!tw_ethereum_abi_decode_params returned an invalid output");

    assert_eq!(output.error, AbiErrorKind::OK);
    assert!(output.error_message.is_empty());

    let expected_tokens = vec![
        named_token(
            "to",
            TokenEnum::address("0x88341d1a8F672D2780C8dC725902AAe72F143B0c".into()),
        ),
        named_token("approved", TokenEnum::boolean(true)),
    ];
    assert_eq!(output.tokens, expected_tokens);
}

#[test]
fn test_ethereum_abi_function_get_signature() {
    let input = AbiProto::FunctionGetTypeInput {
        function_name: "baz".into(),
        inputs: vec![
            param(
                "foo",
                ParamTypeEnum::number_uint(Proto::NumberNType { bits: 64 }),
            ),
            param("bar", ParamTypeEnum::address(Proto::AddressType {})),
        ],
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let actual = TWStringHelper::wrap(unsafe {
        tw_ethereum_abi_function_get_signature(CoinType::Ethereum as u32, input_data.ptr())
    })
    .to_string()
    .expect("!tw_ethereum_abi_function_get_signature returned nullptr");

    assert_eq!(actual, "baz(uint64,address)");
}

#[test]
fn test_ethereum_abi_encode_function() {
    let input = AbiProto::FunctionEncodingInput {
        function_name: "baz".into(),
        tokens: vec![
            named_token("", TokenEnum::number_uint(number_n::<256>(69))),
            named_token("", TokenEnum::boolean(true)),
        ],
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output_data = TWDataHelper::wrap(unsafe {
        tw_ethereum_abi_encode_function(CoinType::Ethereum as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_ethereum_abi_encode_function returned nullptr");

    let output: AbiProto::FunctionEncodingOutput = deserialize(&output_data)
        .expect("!tw_ethereum_abi_encode_function returned an invalid output");

    assert_eq!(output.error, AbiErrorKind::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(output.encoded.to_hex(), "72ed38b600000000000000000000000000000000000000000000000000000000000000450000000000000000000000000000000000000000000000000000000000000001");
}

#[test]
fn test_ethereum_abi_decode_value() {
    let input = AbiProto::ValueDecodingInput {
        encoded: "000000000000000000000000000000000000000000000000000000000000002a"
            .decode_hex()
            .unwrap()
            .into(),
        param_type: "int8".into(),
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output_data = TWDataHelper::wrap(unsafe {
        tw_ethereum_abi_decode_value(CoinType::Ethereum as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_ethereum_abi_decode_value returned nullptr");

    let output: AbiProto::ValueDecodingOutput = deserialize(&output_data)
        .expect("!tw_ethereum_abi_decode_value returned an invalid output");

    assert_eq!(output.error, AbiErrorKind::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(output.param_str, "42");
}
