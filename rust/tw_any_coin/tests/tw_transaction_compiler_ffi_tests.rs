// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::borrow::Cow;
use tw_any_coin::ffi::tw_transaction_compiler::{
    tw_transaction_compiler_build_input, tw_transaction_compiler_compile,
    tw_transaction_compiler_pre_image_hashes,
};
use tw_coin_entry::error::SigningErrorType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_data_vector_helper::TWDataVectorHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use tw_number::U256;
use tw_proto::Ethereum::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_proto::{deserialize, serialize};

const ETHEREUM_COIN_TYPE: u32 = 60;

#[test]
fn test_transaction_compiler_eth() {
    let from = TWStringHelper::create("0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F");
    let to = TWStringHelper::create("0x3535353535353535353535353535353535353535");
    let amount = TWStringHelper::create("1000000000000000000");
    let asset = TWStringHelper::create("ETH");
    let memo = TWStringHelper::create("");
    let chain_id = TWStringHelper::create("");
    let res = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_build_input(
            ETHEREUM_COIN_TYPE,
            from.ptr(),
            to.ptr(),
            amount.ptr(),
            asset.ptr(),
            memo.ptr(),
            chain_id.ptr(),
        )
    })
    .to_vec()
    .expect("!tw_transaction_compiler_build_input returned nullptr");

    let mut input: Proto::SigningInput =
        deserialize(&res).expect("Coin entry returned an invalid output");

    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(1_000_000_000_000_000_000),
        data: Cow::default(),
    };
    let expected_input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(1),
        to_address: Cow::from("0x3535353535353535353535353535353535353535"),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::transfer(transfer),
        }),
        ..Proto::SigningInput::default()
    };
    assert_eq!(input, expected_input);

    // Set a few other values.
    input.nonce = U256::encode_be_compact(11);
    input.gas_price = U256::encode_be_compact(20_000_000_000);
    input.gas_limit = U256::encode_be_compact(21_000);
    input.tx_mode = Proto::TransactionMode::Legacy;

    // Step 2: Obtain preimage hash
    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let preimage_data = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_pre_image_hashes(ETHEREUM_COIN_TYPE, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_transaction_compiler_pre_image_hashes returned nullptr");

    let preimage: CompilerProto::PreSigningOutput =
        deserialize(&preimage_data).expect("Coin entry returned an invalid output");

    assert_eq!(preimage.error, SigningErrorType::OK);
    assert!(preimage.error_message.is_empty());
    assert_eq!(
        preimage.data_hash.to_hex(),
        "15e180a6274b2f6a572b9b51823fce25ef39576d10188ecdcd7de44526c47217"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server
    let signature = "360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07b53bd9d1376e15a191d844db458893b928f3efbfee90c9febf51ab84c9796677900".decode_hex().unwrap();
    let public_key = "044bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382ce28cab79ad7119ee1ad3ebcdb98a16805211530ecc6cfefa1b88e6dff99232a".decode_hex().unwrap();

    let signatures = TWDataVectorHelper::create([signature]);
    let public_keys = TWDataVectorHelper::create([public_key]);

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let output_data = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_compile(
            ETHEREUM_COIN_TYPE,
            input_data.ptr(),
            signatures.ptr(),
            public_keys.ptr(),
        )
    })
    .to_vec()
    .expect("!tw_transaction_compiler_compile returned nullptr");

    let output: Proto::SigningOutput =
        deserialize(&output_data).expect("Coin entry returned an invalid output");

    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    let expected_encoded = "f86c0b8504a817c800825208943535353535353535353535353535353535353535880de0b6b3a76400008025a0360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07ba053bd9d1376e15a191d844db458893b928f3efbfee90c9febf51ab84c97966779";
    assert_eq!(output.encoded.to_hex(), expected_encoded);
}
