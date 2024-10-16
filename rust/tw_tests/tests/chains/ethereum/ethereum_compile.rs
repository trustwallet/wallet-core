// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_any_coin::ffi::tw_any_signer::tw_any_signer_plan;
use tw_any_coin::ffi::tw_transaction_compiler::{
    tw_transaction_compiler_compile, tw_transaction_compiler_pre_image_hashes,
};
use tw_coin_entry::error::prelude::*;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_data_vector_helper::TWDataVectorHelper;
use tw_number::U256;
use tw_proto::Ethereum::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_proto::{deserialize, serialize};

#[test]
fn test_transaction_compiler_eth() {
    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(1_000_000_000_000_000_000),
        data: Cow::default(),
    };
    let input = Proto::SigningInput {
        nonce: U256::encode_be_compact(11),
        chain_id: U256::encode_be_compact(1),
        gas_price: U256::encode_be_compact(20_000_000_000),
        gas_limit: U256::encode_be_compact(21_000),
        to_address: "0x3535353535353535353535353535353535353535".into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::transfer(transfer),
        }),
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let preimage_data = TWDataHelper::wrap(unsafe {
        tw_transaction_compiler_pre_image_hashes(CoinType::Ethereum as u32, input_data.ptr())
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
            CoinType::Ethereum as u32,
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

#[test]
fn test_transaction_compiler_plan_not_supported() {
    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(1),
        data: Cow::default(),
    };
    let input = Proto::SigningInput {
        to_address: "0x3535353535353535353535353535353535353535".into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::transfer(transfer),
        }),
        ..Proto::SigningInput::default()
    };
    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let plan = TWDataHelper::wrap(unsafe {
        tw_any_signer_plan(input_data.ptr(), CoinType::Ethereum as u32)
    });
    assert!(
        plan.is_null(),
        "Transaction plan is expected to be not supported by the {:?} coin",
        CoinType::Ethereum
    );
}
