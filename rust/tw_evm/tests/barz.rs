// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex;
use tw_evm::abi::prebuild::erc20::Erc20;
use tw_evm::address::Address;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::signer::Signer;
use tw_number::U256;
use tw_proto::Ethereum::Proto;

// https://testnet.bscscan.com/tx/0x43fc13dfdf06bbb09da8ce070953753764f1e43782d0c8b621946d8b45749419
#[test]
fn test_barz_transfer_account_deployed() {
    let private_key =
        hex::decode("0x3c90badc15c4d35733769093d3733501e92e7f16e101df284cee9a310d36c483").unwrap();

    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(0x23_86f2_6fc1_0000),
        data: Cow::default(),
    };
    let user_op = Proto::UserOperation {
        entry_point: "0x5FF137D4b0FDCD49DcA30c7CF57E578a026d2789".into(),
        init_code: Cow::default(),
        sender: "0xb16Db98B365B1f89191996942612B14F1Da4Bd5f".into(),
        pre_verification_gas: U256::encode_be_compact(0xb708),
        verification_gas_limit: U256::encode_be_compact(0x186a0),
        paymaster_and_data: Cow::default(),
    };

    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(97),
        nonce: U256::encode_be_compact(2),
        tx_mode: Proto::TransactionMode::UserOp,
        gas_limit: U256::encode_be_compact(0x186A0),
        max_fee_per_gas: U256::encode_be_compact(0x1_a339_c9e9),
        max_inclusion_fee_per_gas: U256::encode_be_compact(0x1_a339_c9e9),
        to_address: "0x61061fCAE11fD5461535e134EfF67A98CFFF44E9".into(),
        private_key: private_key.into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::transfer(transfer),
        }),
        user_operation: Some(user_op),
        ..Proto::SigningInput::default()
    };

    let output = Signer::<StandardEvmContext>::sign_proto(input);
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    let expected = r#"{"callData":"0xb61d27f600000000000000000000000061061fcae11fd5461535e134eff67a98cfff44e9000000000000000000000000000000000000000000000000002386f26fc1000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000","callGasLimit":"100000","initCode":"0x","maxFeePerGas":"7033440745","maxPriorityFeePerGas":"7033440745","nonce":"2","paymasterAndData":"0x","preVerificationGas":"46856","sender":"0xb16Db98B365B1f89191996942612B14F1Da4Bd5f","signature":"0x80e84992ebf8d5f71180231163ed150a7557ed0aa4b4bcee23d463a09847e4642d0fbf112df2e5fa067adf4b2fa17fc4a8ac172134ba5b78e3ec9c044e7f28d71c","verificationGasLimit":"100000"}"#;
    let actual = String::from_utf8(output.encoded.to_vec()).unwrap();
    assert_eq!(actual, expected);

    assert_eq!(
        hex::encode(output.pre_hash, false),
        "2d37191a8688f69090451ed90a0a9ba69d652c2062ee9d023b3ebe964a3ed2ae"
    );
}

// // https://testnet.bscscan.com/tx/0xea1f5cddc0653e116327cbcb3bc770360a642891176eff2ec69c227e46791c31
#[test]
fn test_barz_transfer_account_not_deployed() {
    let private_key =
        hex::decode("0x3c90badc15c4d35733769093d3733501e92e7f16e101df284cee9a310d36c483").unwrap();

    // TODO generate `init_code` with Barz when it's moved to Rust.
    // See: https://github.com/trustwallet/wallet-core/blob/f266567e913a40bfee80b8ecdd4b74785cea2b32/tests/chains/Ethereum/BarzTests.cpp#L160
    let init_code = hex::decode("3fc708630d85a3b5ec217e53100ec2b735d4f800296601cd0000000000000000000000005034534efe9902779ed6ea6983f435c00f3bc51000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004104b173a6a812025c40c38bac46343646bd0a8137c807aae6e04aac238cc24d2ad2116ca14d23d357588ff2aabd7db29d5976f4ecc8037775db86f67e873a306b1f00000000000000000000000000000000000000000000000000000000000000").unwrap();

    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(0x23_86f2_6fc1_0000),
        data: Cow::default(),
    };
    let user_op = Proto::UserOperation {
        entry_point: "0x5FF137D4b0FDCD49DcA30c7CF57E578a026d2789".into(),
        init_code: init_code.into(),
        sender: "0x1392Ae041BfBdBAA0cFF9234a0C8F64df97B7218".into(),
        pre_verification_gas: U256::encode_be_compact(0xb708),
        verification_gas_limit: U256::encode_be_compact(0x2D_C6C0),
        paymaster_and_data: Cow::default(),
    };

    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(97),
        nonce: U256::encode_be_compact(0),
        tx_mode: Proto::TransactionMode::UserOp,
        gas_limit: U256::encode_be_compact(0x26_25A0),
        max_fee_per_gas: U256::encode_be_compact(0x1_a339_c9e9),
        max_inclusion_fee_per_gas: U256::encode_be_compact(0x1_a339_c9e9),
        to_address: "0x61061fCAE11fD5461535e134EfF67A98CFFF44E9".into(),
        private_key: private_key.into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::transfer(transfer),
        }),
        user_operation: Some(user_op),
        ..Proto::SigningInput::default()
    };

    let output = Signer::<StandardEvmContext>::sign_proto(input);
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    let expected = r#"{"callData":"0xb61d27f600000000000000000000000061061fcae11fd5461535e134eff67a98cfff44e9000000000000000000000000000000000000000000000000002386f26fc1000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000","callGasLimit":"2500000","initCode":"0x3fc708630d85a3b5ec217e53100ec2b735d4f800296601cd0000000000000000000000005034534efe9902779ed6ea6983f435c00f3bc51000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004104b173a6a812025c40c38bac46343646bd0a8137c807aae6e04aac238cc24d2ad2116ca14d23d357588ff2aabd7db29d5976f4ecc8037775db86f67e873a306b1f00000000000000000000000000000000000000000000000000000000000000","maxFeePerGas":"7033440745","maxPriorityFeePerGas":"7033440745","nonce":"0","paymasterAndData":"0x","preVerificationGas":"46856","sender":"0x1392Ae041BfBdBAA0cFF9234a0C8F64df97B7218","signature":"0xbf1b68323974e71ad9bd6dfdac07dc062599d150615419bb7876740d2bcf3c8909aa7e627bb0e08a2eab930e2e7313247c9b683c884236dd6ea0b6834fb2cb0a1b","verificationGasLimit":"3000000"}"#;
    let actual = String::from_utf8(output.encoded.to_vec()).unwrap();
    assert_eq!(actual, expected);

    assert_eq!(
        hex::encode(output.pre_hash, false),
        "548c13a0bb87981d04a3a24a78ad5e4ba8d0afbf3cfe9311250e07b54cd38937"
    );
}

// // https://testnet.bscscan.com/tx/0xea1f5cddc0653e116327cbcb3bc770360a642891176eff2ec69c227e46791c31
#[test]
fn test_barz_batched_account_deployed() {
    let private_key =
        hex::decode("0x3c90badc15c4d35733769093d3733501e92e7f16e101df284cee9a310d36c483").unwrap();

    let contract_address = "0x03bBb5660B8687C2aa453A0e42dCb6e0732b1266";

    let mut calls = Vec::with_capacity(2);

    // ERC20 approve
    {
        let spender = Address::from("0x5FF137D4b0FDCD49DcA30c7CF57E578a026d2789");
        let amount = U256::from(0x8AC7_2304_89E8_0000_u64);
        let payload = Erc20::approve(spender, amount).unwrap();

        calls.push(Proto::mod_Transaction::mod_Batch::BatchedCall {
            address: contract_address.into(),
            amount: Cow::default(),
            payload: payload.into(),
        });
    };

    // ERC20 transfer
    {
        let recipient = Address::from("0x5FF137D4b0FDCD49DcA30c7CF57E578a026d2789");
        let amount = U256::from(0x8AC7_2304_89E8_0000_u64);
        let payload = Erc20::transfer(recipient, amount).unwrap();

        calls.push(Proto::mod_Transaction::mod_Batch::BatchedCall {
            address: contract_address.into(),
            amount: Cow::default(),
            payload: payload.into(),
        });
    }

    let user_op = Proto::UserOperation {
        entry_point: "0x5FF137D4b0FDCD49DcA30c7CF57E578a026d2789".into(),
        init_code: Cow::default(),
        sender: "0x1e6c542ebc7c960c6a155a9094db838cef842cf5".into(),
        pre_verification_gas: U256::encode_be_compact(0xDAFC),
        verification_gas_limit: U256::encode_be_compact(0x07_F7C4),
        paymaster_and_data: Cow::default(),
    };

    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(97),
        nonce: U256::encode_be_compact(3),
        tx_mode: Proto::TransactionMode::UserOp,
        gas_limit: U256::encode_be_compact(0x01_5A61),
        max_fee_per_gas: U256::encode_be_compact(0x02_540B_E400),
        max_inclusion_fee_per_gas: U256::encode_be_compact(0x02_540B_E400),
        to_address: contract_address.into(),
        private_key: private_key.into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::batch(
                Proto::mod_Transaction::Batch { calls },
            ),
        }),
        user_operation: Some(user_op),
        ..Proto::SigningInput::default()
    };

    let output = Signer::<StandardEvmContext>::sign_proto(input);
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    let expected = r#"{"callData":"0x47e1da2a000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000c00000000000000000000000000000000000000000000000000000000000000120000000000000000000000000000000000000000000000000000000000000000200000000000000000000000003bbb5660b8687c2aa453a0e42dcb6e0732b126600000000000000000000000003bbb5660b8687c2aa453a0e42dcb6e0732b12660000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000c00000000000000000000000000000000000000000000000000000000000000044095ea7b30000000000000000000000005ff137d4b0fdcd49dca30c7cf57e578a026d27890000000000000000000000000000000000000000000000008ac7230489e80000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000044a9059cbb0000000000000000000000005ff137d4b0fdcd49dca30c7cf57e578a026d27890000000000000000000000000000000000000000000000008ac7230489e8000000000000000000000000000000000000000000000000000000000000","callGasLimit":"88673","initCode":"0x","maxFeePerGas":"10000000000","maxPriorityFeePerGas":"10000000000","nonce":"3","paymasterAndData":"0x","preVerificationGas":"56060","sender":"0x1E6c542ebC7c960c6A155A9094DB838cEf842cf5","signature":"0x0747b665fe9f3a52407f95a35ac3e76de37c9b89483ae440431244e89a77985f47df712c7364c1a299a5ef62d0b79a2cf4ed63d01772275dd61f72bd1ad5afce1c","verificationGasLimit":"522180"}"#;
    let actual = String::from_utf8(output.encoded.to_vec()).unwrap();
    assert_eq!(actual, expected);

    assert_eq!(
        hex::encode(output.pre_hash, false),
        "84d0464f5a2b191e06295443970ecdcd2d18f565d0d52b5a79443192153770ab"
    );
}
