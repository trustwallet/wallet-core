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
use tw_encoding::hex;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_data_vector_helper::TWDataVectorHelper;
use tw_misc::assert_eq_json;
use tw_number::U256;
use tw_proto::Ethereum::Proto;
use tw_proto::Ethereum::Proto::{
    Authorization, AuthorizationCustomSignature, TransactionMode, UserOperationV0_7,
};
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
fn test_transaction_compiler_eip7702() {
    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(0),
        data: Cow::default(),
    };
    let input = Proto::SigningInput {
        tx_mode: TransactionMode::SetCode,
        nonce: U256::encode_be_compact(0),
        chain_id: U256::encode_be_compact(17000), // Holesky Testnet
        max_inclusion_fee_per_gas: U256::encode_be_compact(20_000_000_000),
        max_fee_per_gas: U256::encode_be_compact(20_000_000_000),
        gas_limit: U256::encode_be_compact(50_000),
        to_address: "0x18356de2Bc664e45dD22266A674906574087Cf54".into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::transfer(transfer),
        }),
        eip7702_authorization: Some(Authorization {
            address: "0x3535353535353535353535353535353535353535".into(),
            custom_signature: Some(AuthorizationCustomSignature {
                nonce: U256::encode_be_compact(1),
                chain_id: U256::encode_be_compact(0), // chain id 0 means any chain
                signature: "08b7bfc6bcaca1dfd7a295c3a6908fea545a62958cf2c048639224a8bede8d1f56dce327574529c56f7f3db308a34d44e2312a11c89db8af99371d4fe490e55f00".into(),
            }),
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
        "d5f3f94ff2c70686623c71ef9e367f341a3ef6691d02e9e9db671c9281d56432"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server
    let signature = "601148c0af0108fe9f051ca5e9bd5c0b9e7c4cc7385b1625eeae0ec4bbe5537960d8c76d98279329d1ef9d79002ccddd53e522c0e821003590e74eba51a1c95601".decode_hex().unwrap();
    let public_key = "04ec6632291fbfe6b47826a1c4b195f8b112a7e147e8a5a15fb0f7d7de022652e7f65b97a57011c09527688e23c07ae9c83a2cae2e49edba226e7c43f0baa7296d".decode_hex().unwrap();

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
    let expected_encoded = "04f8cc824268808504a817c8008504a817c80082c3509418356de2bc664e45dd22266a674906574087cf548080c0f85cf85a809435353535353535353535353535353535353535350180a008b7bfc6bcaca1dfd7a295c3a6908fea545a62958cf2c048639224a8bede8d1fa056dce327574529c56f7f3db308a34d44e2312a11c89db8af99371d4fe490e55f01a0601148c0af0108fe9f051ca5e9bd5c0b9e7c4cc7385b1625eeae0ec4bbe55379a060d8c76d98279329d1ef9d79002ccddd53e522c0e821003590e74eba51a1c956";
    // Successfully broadcasted: https://holesky.etherscan.io/tx/0x1c349e81dd135bfe104c8051fa9e668d6f8c0323abe852b1d1522b772932c0ec
    assert_eq!(output.encoded.to_hex(), expected_encoded);
}

#[test]
fn test_transaction_compiler_user_op_v0_7_with_eip7702() {
    // Step 1: Prepare the input
    let contract_generic = Proto::mod_Transaction::ContractGeneric {
        amount: U256::encode_be_compact(0),
        // call function: execute(address target, uint256 value, bytes calldata data)
        data: hex::decode("0xb61d27f60000000000000000000000003a4a63b8763749c4cd30909becfd0e68364870a400000000000000000000000000000000000000000000000000000000000004d200000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000").unwrap().into(),
    };
    let input = Proto::SigningInput {
        tx_mode: TransactionMode::UserOp,
        nonce: U256::encode_be_compact(2),
        chain_id: U256::encode_be_compact(11155111),
        gas_limit: U256::from(600000u128).to_big_endian_compact().into(),
        max_inclusion_fee_per_gas: U256::from(100000000u128).to_big_endian_compact().into(),
        max_fee_per_gas: U256::from(939734933u128).to_big_endian_compact().into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::contract_generic(
                contract_generic,
            ),
        }),
        user_operation_oneof:
            Proto::mod_SigningInput::OneOfuser_operation_oneof::user_operation_v0_7(
                UserOperationV0_7 {
                    entry_point: "0x0000000071727De22E5E9d8BAf0edAc6f37da032".into(),
                    factory: Default::default(),
                    factory_data: "".decode_hex().unwrap().into(),
                    sender: "0x18356de2Bc664e45dD22266A674906574087Cf54".into(),
                    pre_verification_gas: U256::from(400000u64).to_big_endian_compact().into(),
                    verification_gas_limit: U256::from(1000000u128).to_big_endian_compact().into(),
                    paymaster: Cow::from(""),
                    paymaster_verification_gas_limit: Vec::new().into(),
                    paymaster_post_op_gas_limit: Vec::new().into(),
                    paymaster_data: "".decode_hex().unwrap().into(),
                },
            ),
        eip7702_authorization: Some(Authorization {
            address: "0xbA0db987220ecE42a60EE9f48FDE633E6c938482".into(),
            custom_signature: Some(AuthorizationCustomSignature {
                nonce: U256::encode_be_compact(19),
                chain_id: U256::encode_be_compact(11155111),
                signature: "7b2abdf6d874487751f3efd4d4eb8d39a50569c656f5863cf4de064caafa0e106954dfea7fa4e91e8d20c08e78b4cb2e21a7e56072b02d622fbb5637b5a0d9d701".into(),
            }),
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
        "3bf2bf012750e35a16e3430b010f2eb2dda8309892309afce6714196d4912d56" // The AA user op hash
    );

    // Step 3: Compile transaction info
    // Simulate signature, normally obtained from signature server
    let signature = "4143bd278c97a4738846d9bc5756d0433bd062ea321cafb5ad3f7e3f68c68385277444f6fc851401ab319dad3756cd9e83c3ba9dfa502a69c43827fc2267f6d701".decode_hex().unwrap();
    let public_key = "04ec6632291fbfe6b47826a1c4b195f8b112a7e147e8a5a15fb0f7d7de022652e7f65b97a57011c09527688e23c07ae9c83a2cae2e49edba226e7c43f0baa7296d".decode_hex().unwrap();

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

    let expected_encoded = r#"
{
	"sender": "0x18356de2Bc664e45dD22266A674906574087Cf54",
	"nonce": "0x02",
	"callData": "0xb61d27f60000000000000000000000003a4a63b8763749c4cd30909becfd0e68364870a400000000000000000000000000000000000000000000000000000000000004d200000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000",
	"callGasLimit": "0x0927c0",
	"verificationGasLimit": "0x0f4240",
	"preVerificationGas": "0x061a80",
	"maxFeePerGas": "0x38033795",
	"maxPriorityFeePerGas": "0x05f5e100",
	"paymasterVerificationGasLimit": "0x00",
	"paymasterPostOpGasLimit": "0x00",
	"eip7702Auth": {
		"chainId": "0xaa36a7",
		"address": "0xbA0db987220ecE42a60EE9f48FDE633E6c938482",
		"nonce": "0x13",
		"yParity": "0x01",
		"r": "0x7b2abdf6d874487751f3efd4d4eb8d39a50569c656f5863cf4de064caafa0e10",
		"s": "0x6954dfea7fa4e91e8d20c08e78b4cb2e21a7e56072b02d622fbb5637b5a0d9d7"
	},
	"signature": "0x4143bd278c97a4738846d9bc5756d0433bd062ea321cafb5ad3f7e3f68c68385277444f6fc851401ab319dad3756cd9e83c3ba9dfa502a69c43827fc2267f6d71c"
}
    "#;
    // Successfully broadcast by a bundler:
    // https://sepolia.etherscan.io/tx/0x7b89079050a3f3f5afadaeda93750ed797132eb35e038962d15a18f9e862bb58
    // The AA Transaction:
    // https://sepolia.etherscan.io/tx/0x3bf2bf012750e35a16e3430b010f2eb2dda8309892309afce6714196d4912d56
    assert_eq_json!(
        std::str::from_utf8(output.encoded.iter().as_slice()).unwrap(),
        expected_encoded
    );
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
