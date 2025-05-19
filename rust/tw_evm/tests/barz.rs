// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_evm::abi::prebuild::erc20::Erc20;
use tw_evm::address::Address;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::signer::Signer;
use tw_misc::traits::ToBytesVec;
use tw_number::U256;
use tw_proto::Ethereum::Proto;
use tw_proto::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof as TransactionType;

fn execute(tx: TransactionType, wallet_type: Proto::SCWalletType) -> Proto::Transaction {
    Proto::Transaction {
        transaction_oneof: TransactionType::scw_execute(Box::new(
            Proto::mod_Transaction::SCWalletExecute {
                transaction: Some(Box::new(Proto::Transaction {
                    transaction_oneof: tx,
                })),
                wallet_type,
            },
        )),
    }
}

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
        transaction: Some(execute(
            TransactionType::transfer(transfer),
            Proto::SCWalletType::SimpleAccount,
        )),
        user_operation_oneof: Proto::mod_SigningInput::OneOfuser_operation_oneof::user_operation(
            user_op,
        ),
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
        transaction: Some(execute(
            TransactionType::transfer(transfer),
            Proto::SCWalletType::SimpleAccount,
        )),
        user_operation_oneof: Proto::mod_SigningInput::OneOfuser_operation_oneof::user_operation(
            user_op,
        ),
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

        calls.push(Proto::mod_Transaction::mod_SCWalletBatch::BatchedCall {
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

        calls.push(Proto::mod_Transaction::mod_SCWalletBatch::BatchedCall {
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
            transaction_oneof: TransactionType::scw_batch(Proto::mod_Transaction::SCWalletBatch {
                calls,
                wallet_type: Proto::SCWalletType::SimpleAccount,
            }),
        }),
        user_operation_oneof: Proto::mod_SigningInput::OneOfuser_operation_oneof::user_operation(
            user_op,
        ),
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

#[test]
fn test_barz_transfer_account_not_deployed_v0_7() {
    let private_key =
        hex::decode("0x3c90badc15c4d35733769093d3733501e92e7f16e101df284cee9a310d36c483").unwrap();

    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(0x23_86f2_6fc1_0000),
        data: Cow::default(),
    };
    let user_op = Proto::UserOperationV0_7 {
        entry_point: "0x0000000071727De22E5E9d8BAf0edAc6f37da032".into(),
        sender: "0x174a240e5147D02dE4d7724D5D3E1c1bF11cE029".into(),
        pre_verification_gas: U256::from(1000000u64).to_big_endian_compact().into(),
        verification_gas_limit: U256::from(100000u128).to_big_endian_compact().into(),
        factory: "0xf471789937856d80e589f5996cf8b0511ddd9de4".into(),
        factory_data: "f471789937856d80e589f5996cf8b0511ddd9de4".decode_hex().unwrap().into(),
        paymaster: "0xf62849f9a0b5bf2913b396098f7c7019b51a820a".into(),
        paymaster_verification_gas_limit: U256::from(99999u128).to_big_endian_compact().into(),
        paymaster_post_op_gas_limit: U256::from(88888u128).to_big_endian_compact().into(),
        paymaster_data: "00000000000b0000000000002e234dae75c793f67a35089c9d99245e1c58470b00000000000000000000000000000000000000000000000000000000000186a0072f35038bcacc31bcdeda87c1d9857703a26fb70a053f6e87da5a4e7a1e1f3c4b09fbe2dbff98e7a87ebb45a635234f4b79eff3225d07560039c7764291c97e1b".decode_hex().unwrap().into(),
    };

    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(31337u64),
        nonce: U256::encode_be_compact(0u64),
        tx_mode: Proto::TransactionMode::UserOp,
        gas_limit: U256::from(100000u128).to_big_endian_compact().into(),
        max_fee_per_gas: U256::from(100000u128).to_big_endian_compact().into(),
        max_inclusion_fee_per_gas: U256::from(100000u128).to_big_endian_compact().into(),
        to_address: "0x61061fCAE11fD5461535e134EfF67A98CFFF44E9".into(),
        private_key: private_key.into(),
        transaction: Some(execute(
            TransactionType::transfer(transfer),
            Proto::SCWalletType::SimpleAccount,
        )),
        user_operation_oneof:
            Proto::mod_SigningInput::OneOfuser_operation_oneof::user_operation_v0_7(user_op),
        ..Proto::SigningInput::default()
    };

    let output = Signer::<StandardEvmContext>::sign_proto(input);
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    assert_eq!(
        hex::encode(output.pre_hash, false),
        "f177858c1c500e51f38ffe937bed7e4d3a8678725900be4682d3ce04d97071eb"
    );
}

#[test]
fn test_biz4337_transfer() {
    let private_key =
        hex::decode("0x3c90badc15c4d35733769093d3733501e92e7f16e101df284cee9a310d36c483").unwrap();

    let user_op = Proto::UserOperationV0_7 {
        entry_point: "0x0000000071727De22E5E9d8BAf0edAc6f37da032".into(),
        sender: "0x2EF648D7C03412B832726fd4683E2625deA047Ba".into(),
        pre_verification_gas: U256::from(1000000u64).to_big_endian_compact().into(),
        verification_gas_limit: U256::from(100000u128).to_big_endian_compact().into(),
        paymaster: "0xb0086171AC7b6BD4D046580bca6d6A4b0835c232".into(),
        paymaster_verification_gas_limit: U256::from(99999u128).to_big_endian_compact().into(),
        paymaster_post_op_gas_limit: U256::from(88888u128).to_big_endian_compact().into(),
        // Dummy paymaster data.
        paymaster_data: "00000000000b0000000000002e234dae75c793f67a35089c9d99245e1c58470b00000000000000000000000000000000000000000000000000000000000186a0072f35038bcacc31bcdeda87c1d9857703a26fb70a053f6e87da5a4e7a1e1f3c4b09fbe2dbff98e7a87ebb45a635234f4b79eff3225d07560039c7764291c97e1b".decode_hex().unwrap().into(),
        ..Proto::UserOperationV0_7::default()
    };

    let approve = Proto::mod_Transaction::ERC20Approve {
        // Paymaster address.
        spender: "0xb0086171AC7b6BD4D046580bca6d6A4b0835c232".into(),
        amount: U256::encode_be_compact(655_360_197_115_136_u64),
    };
    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(31337u64),
        nonce: U256::encode_be_compact(0u64),
        tx_mode: Proto::TransactionMode::UserOp,
        gas_limit: U256::from(100000u128).to_big_endian_compact().into(),
        max_fee_per_gas: U256::from(100000u128).to_big_endian_compact().into(),
        max_inclusion_fee_per_gas: U256::from(100000u128).to_big_endian_compact().into(),
        // USDT token.
        to_address: "0xdac17f958d2ee523a2206206994597c13d831ec7".into(),
        private_key: private_key.into(),
        transaction: Some(execute(
            TransactionType::erc20_approve(approve),
            Proto::SCWalletType::Biz4337,
        )),
        user_operation_oneof:
            Proto::mod_SigningInput::OneOfuser_operation_oneof::user_operation_v0_7(user_op),
        ..Proto::SigningInput::default()
    };

    let output = Signer::<StandardEvmContext>::sign_proto(input);
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    assert_eq!(
        hex::encode(output.pre_hash, false),
        "68109b9caf49f7971b689307c9a77ceec46e4b8fa88421c4276dd846f782d92c"
    );

    let expected = r#"{"sender":"0x2EF648D7C03412B832726fd4683E2625deA047Ba","nonce":"0x00","callData":"0x76276c82000000000000000000000000dac17f958d2ee523a2206206994597c13d831ec7000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044095ea7b3000000000000000000000000b0086171ac7b6bd4d046580bca6d6a4b0835c2320000000000000000000000000000000000000000000000000002540befbfbd0000000000000000000000000000000000000000000000000000000000","callGasLimit":"0x0186a0","verificationGasLimit":"0x0186a0","preVerificationGas":"0x0f4240","maxFeePerGas":"0x0186a0","maxPriorityFeePerGas":"0x0186a0","paymaster":"0xb0086171AC7b6BD4D046580bca6d6A4b0835c232","paymasterVerificationGasLimit":"0x01869f","paymasterPostOpGasLimit":"0x015b38","paymasterData":"0x00000000000b0000000000002e234dae75c793f67a35089c9d99245e1c58470b00000000000000000000000000000000000000000000000000000000000186a0072f35038bcacc31bcdeda87c1d9857703a26fb70a053f6e87da5a4e7a1e1f3c4b09fbe2dbff98e7a87ebb45a635234f4b79eff3225d07560039c7764291c97e1b","signature":"0xf6b1f7ad22bcc68ca292bc10d15e82e0eab8c75c1a04f9750e7cff1418d38d9c6c115c510e3f47eb802103d62f88fa7d4a3b2e24e2ddbe7ee68153920ab3f6cc1b"}"#;
    let actual = String::from_utf8(output.encoded.to_vec()).unwrap();
    assert_eq!(actual, expected);
}

#[test]
fn test_biz4337_transfer_batch() {
    let private_key =
        hex::decode("0x3c90badc15c4d35733769093d3733501e92e7f16e101df284cee9a310d36c483").unwrap();

    let user_op = Proto::UserOperationV0_7 {
        entry_point: "0x0000000071727De22E5E9d8BAf0edAc6f37da032".into(),
        sender: "0x2EF648D7C03412B832726fd4683E2625deA047Ba".into(),
        pre_verification_gas: U256::from(1000000u64).to_big_endian_compact().into(),
        verification_gas_limit: U256::from(100000u128).to_big_endian_compact().into(),
        paymaster: "0xb0086171AC7b6BD4D046580bca6d6A4b0835c232".into(),
        paymaster_verification_gas_limit: U256::from(99999u128).to_big_endian_compact().into(),
        paymaster_post_op_gas_limit: U256::from(88888u128).to_big_endian_compact().into(),
        // Dummy paymaster data.
        paymaster_data: "00000000000b0000000000002e234dae75c793f67a35089c9d99245e1c58470b00000000000000000000000000000000000000000000000000000000000186a0072f35038bcacc31bcdeda87c1d9857703a26fb70a053f6e87da5a4e7a1e1f3c4b09fbe2dbff98e7a87ebb45a635234f4b79eff3225d07560039c7764291c97e1b".decode_hex().unwrap().into(),
        ..Proto::UserOperationV0_7::default()
    };

    let mut calls = Vec::with_capacity(2);

    // ERC20 approve. At least one of the calls should be an ERC20.approve()
    // so paymaster can collect tokens to cover the fees.
    {
        // Paymaster address.
        let recipient = Address::from("0xb0086171AC7b6BD4D046580bca6d6A4b0835c232");
        let amount = U256::from(655_360_197_115_136_u64);
        let payload = Erc20::approve(recipient, amount).unwrap();

        calls.push(Proto::mod_Transaction::mod_SCWalletBatch::BatchedCall {
            // USDT
            address: "0xdac17f958d2ee523a2206206994597c13d831ec7".into(),
            amount: Cow::default(),
            payload: payload.into(),
        });
    }

    // ERC20 transfer. Regular transaction.
    {
        let recipient = Address::from("0x5FF137D4b0FDCD49DcA30c7CF57E578a026d2789");
        let amount = U256::from(0x8AC7_2304_89E8_0000_u64);
        let payload = Erc20::transfer(recipient, amount).unwrap();

        calls.push(Proto::mod_Transaction::mod_SCWalletBatch::BatchedCall {
            address: "0x03bBb5660B8687C2aa453A0e42dCb6e0732b1266".into(),
            amount: Cow::default(),
            payload: payload.into(),
        });
    }

    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(31337u64),
        nonce: U256::encode_be_compact(0u64),
        tx_mode: Proto::TransactionMode::UserOp,
        gas_limit: U256::from(100000u128).to_big_endian_compact().into(),
        max_fee_per_gas: U256::from(100000u128).to_big_endian_compact().into(),
        max_inclusion_fee_per_gas: U256::from(100000u128).to_big_endian_compact().into(),
        // USDT token.
        to_address: "0xdac17f958d2ee523a2206206994597c13d831ec7".into(),
        private_key: private_key.into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: TransactionType::scw_batch(Proto::mod_Transaction::SCWalletBatch {
                calls,
                wallet_type: Proto::SCWalletType::Biz4337,
            }),
        }),
        user_operation_oneof:
            Proto::mod_SigningInput::OneOfuser_operation_oneof::user_operation_v0_7(user_op),
        ..Proto::SigningInput::default()
    };

    let output = Signer::<StandardEvmContext>::sign_proto(input);
    assert_eq!(
        output.error,
        SigningErrorType::OK,
        "{}",
        output.error_message
    );

    assert_eq!(
        hex::encode(output.pre_hash, false),
        "f6340068891dc3eb78959993151c421dde23982b3a1407c0dbbd62c2c22c3cb8"
    );

    let expected = r#"{"sender":"0x2EF648D7C03412B832726fd4683E2625deA047Ba","nonce":"0x00","callData":"0x26da7d880000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000120000000000000000000000000dac17f958d2ee523a2206206994597c13d831ec7000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044095ea7b3000000000000000000000000b0086171ac7b6bd4d046580bca6d6a4b0835c2320000000000000000000000000000000000000000000000000002540befbfbd000000000000000000000000000000000000000000000000000000000000000000000000000000000003bbb5660b8687c2aa453a0e42dcb6e0732b1266000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044a9059cbb0000000000000000000000005ff137d4b0fdcd49dca30c7cf57e578a026d27890000000000000000000000000000000000000000000000008ac7230489e8000000000000000000000000000000000000000000000000000000000000","callGasLimit":"0x0186a0","verificationGasLimit":"0x0186a0","preVerificationGas":"0x0f4240","maxFeePerGas":"0x0186a0","maxPriorityFeePerGas":"0x0186a0","paymaster":"0xb0086171AC7b6BD4D046580bca6d6A4b0835c232","paymasterVerificationGasLimit":"0x01869f","paymasterPostOpGasLimit":"0x015b38","paymasterData":"0x00000000000b0000000000002e234dae75c793f67a35089c9d99245e1c58470b00000000000000000000000000000000000000000000000000000000000186a0072f35038bcacc31bcdeda87c1d9857703a26fb70a053f6e87da5a4e7a1e1f3c4b09fbe2dbff98e7a87ebb45a635234f4b79eff3225d07560039c7764291c97e1b","signature":"0x21ab0bdcd1441aef3e4046a922bab3636d0c74011c1b055c55ad9f39ae9b4dac59bcbf3bc1ff31b367a83360edfc8e9652f1a5c8b07eb76fe5a426835682d6721c"}"#;
    let actual = String::from_utf8(output.encoded.to_vec()).unwrap();
    assert_eq!(actual, expected);
}

#[test]
fn test_biz_eip7702_transfer() {
    let private_key =
        hex::decode("0xe148e40f06ee3ba316cdb2571f33486cf879c0ffd2b279ce9f9a88c41ce962e7").unwrap();

    let erc20_transfer = Proto::mod_Transaction::ERC20Transfer {
        to: "0x95dc01ebd10b6dccf1cc329af1a3f73806117c2e".into(),
        amount: U256::encode_be_compact(500_000_000_000_000_u64),
    };
    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(56_u64),
        nonce: U256::encode_be_compact(16_u64),
        tx_mode: Proto::TransactionMode::SetCode,
        gas_limit: U256::from(100_000_u128).to_big_endian_compact().into(),
        max_fee_per_gas: U256::from(1_000_000_000_u128)
            .to_big_endian_compact()
            .into(),
        max_inclusion_fee_per_gas: U256::from(1_000_000_000_u128)
            .to_big_endian_compact()
            .into(),
        private_key: private_key.into(),
        transaction: Some(execute(
            TransactionType::erc20_transfer(erc20_transfer),
            Proto::SCWalletType::Biz,
        )),
        // TWT token.
        to_address: "0x4B0F1812e5Df2A09796481Ff14017e6005508003".into(),
        eip7702_authorization: Some(Proto::Authorization {
            address: "0x117BC8454756456A0f83dbd130Bb94D793D3F3F7".into(),
            custom_signature: None,
        }),
        ..Proto::SigningInput::default()
    };

    let output = Signer::<StandardEvmContext>::sign_proto(input);
    assert_eq!(
        output.error,
        SigningErrorType::OK,
        "{}",
        output.error_message
    );

    assert_eq!(
        output.pre_hash.to_hex(),
        "8917c03bdd4be922d2163448902eb4f9be4c1fb427641d10f72331e839b00dce"
    );
    // Successfully broadcasted transaction:
    // https://bscscan.com/tx/0x723c6265ded49520372b4e04d66290fc946f12a48375ee0b1f01165ebe85f0e1
    assert_eq!(
        output.encoded.to_hex(),
        "04f901ae3810843b9aca00843b9aca00830186a0945132829820b44dc3e8586cec926a16fca0a5608480b8e4b61d27f60000000000000000000000004b0f1812e5df2a09796481ff14017e6005508003000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044a9059cbb00000000000000000000000095dc01ebd10b6dccf1cc329af1a3f73806117c2e0000000000000000000000000000000000000000000000000001c6bf5263400000000000000000000000000000000000000000000000000000000000c0f85cf85a3894117bc8454756456a0f83dbd130bb94d793d3f3f71180a0f435b376e77a6baff416c53d83992ff53d65846cb1a21686d6743dceee5e7c21a03a9eff368ecc02f1126facd76e8ae5003528ff48ddec3302ad52b06828e992f001a0303774c304ef92095bddf85dba08ea6c7d31d89adf974fe4bcf68c80aee0200aa0669244d097856a4c91433219ab9530650f7012c6118b537d193ca82de05acaac"
    );
}

#[test]
fn test_biz_eip7702_transfer_batch() {
    let private_key =
        hex::decode("0xe148e40f06ee3ba316cdb2571f33486cf879c0ffd2b279ce9f9a88c41ce962e7").unwrap();

    let mut calls = Vec::with_capacity(2);

    // ERC20 transfer #1.
    {
        let recipient = Address::from("0x2EF648D7C03412B832726fd4683E2625deA047Ba");
        // 0.0001 TWT
        let amount = U256::from(100_000_000_000_000_u64);
        let payload = Erc20::transfer(recipient, amount).unwrap();

        calls.push(Proto::mod_Transaction::mod_SCWalletBatch::BatchedCall {
            // TWT
            address: "0x4B0F1812e5Df2A09796481Ff14017e6005508003".into(),
            amount: Cow::default(),
            payload: payload.into(),
        });
    }

    // ERC20 transfer #2.
    {
        let recipient = Address::from("0x95dc01ebd10b6dccf1cc329af1a3f73806117c2e");
        // 0.0005 TWT
        let amount = U256::from(500_000_000_000_000_u64);
        let payload = Erc20::transfer(recipient, amount).unwrap();

        calls.push(Proto::mod_Transaction::mod_SCWalletBatch::BatchedCall {
            // TWT
            address: "0x4B0F1812e5Df2A09796481Ff14017e6005508003".into(),
            amount: Cow::default(),
            payload: payload.into(),
        });
    }

    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(56_u64),
        nonce: U256::encode_be_compact(18_u64),
        tx_mode: Proto::TransactionMode::SetCode,
        gas_limit: U256::from(100_000_u128).to_big_endian_compact().into(),
        max_fee_per_gas: U256::from(1_000_000_000_u128)
            .to_big_endian_compact()
            .into(),
        max_inclusion_fee_per_gas: U256::from(1_000_000_000_u128)
            .to_big_endian_compact()
            .into(),
        private_key: private_key.into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: TransactionType::scw_batch(Proto::mod_Transaction::SCWalletBatch {
                calls,
                wallet_type: Proto::SCWalletType::Biz,
            }),
        }),
        eip7702_authorization: Some(Proto::Authorization {
            address: "0x117BC8454756456A0f83dbd130Bb94D793D3F3F7".into(),
            custom_signature: None,
        }),
        ..Proto::SigningInput::default()
    };

    let output = Signer::<StandardEvmContext>::sign_proto(input);
    assert_eq!(
        output.error,
        SigningErrorType::OK,
        "{}",
        output.error_message
    );

    assert_eq!(
        output.pre_hash.to_hex(),
        "00b2d13719df301927ddcbdad5b6bc6214f2007c6408df883c9ea483b45e6f44"
    );
    // Successfully broadcasted transaction:
    // https://bscscan.com/tx/0x425eb17a8e1dee2fcee8352a772d83cbb069c2e03f2c5d9d00da3b3ef66ce48b
    assert_eq!(
        output.encoded.to_hex(),
        "04f9030f3812843b9aca00843b9aca00830186a0945132829820b44dc3e8586cec926a16fca0a5608480b9024434fcd5be00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000001200000000000000000000000004b0f1812e5df2a09796481ff14017e6005508003000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044a9059cbb0000000000000000000000002ef648d7c03412b832726fd4683e2625dea047ba00000000000000000000000000000000000000000000000000005af3107a4000000000000000000000000000000000000000000000000000000000000000000000000000000000004b0f1812e5df2a09796481ff14017e6005508003000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044a9059cbb00000000000000000000000095dc01ebd10b6dccf1cc329af1a3f73806117c2e0000000000000000000000000000000000000000000000000001c6bf5263400000000000000000000000000000000000000000000000000000000000c0f85cf85a3894117bc8454756456a0f83dbd130bb94d793d3f3f71380a0073afc661c158a2dccf4183f87e1e4d62b4d406af418cfd69959368ec9bec2a6a064292fd61d4d16b840470a86fc4f7a89413f9126d897f2268eb76a1d887c6d7a01a0e8bcbd96323c9d3e67b74366b2f43299100996d9e8874a6fd87186ac8f580d4ca07c25b4f0619af77fb953e8f0e4372bfbee62616ad419697516108eeb9bcebb28"
    );
}

#[test]
fn test_biz_eip1559_transfer() {
    // 0x6E860086BbA8fdEafB553815aF0F09a854cC887a
    let private_key =
        hex::decode("0xe762e91cc4889a9fce79b2d2ffc079f86c48331f57b2cd16a33bee060fe448e1").unwrap();

    let erc20_transfer = Proto::mod_Transaction::ERC20Transfer {
        to: "0x95dc01ebd10b6dccf1cc329af1a3f73806117c2e".into(),
        amount: U256::encode_be_compact(200_000_000_000_000_u64),
    };
    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(56_u64),
        nonce: U256::encode_be_compact(2_u64),
        tx_mode: Proto::TransactionMode::Enveloped,
        gas_limit: U256::from(100_000_u128).to_big_endian_compact().into(),
        max_fee_per_gas: U256::from(1_000_000_000_u128)
            .to_big_endian_compact()
            .into(),
        max_inclusion_fee_per_gas: U256::from(1_000_000_000_u128)
            .to_big_endian_compact()
            .into(),
        private_key: private_key.into(),
        transaction: Some(execute(
            TransactionType::erc20_transfer(erc20_transfer),
            Proto::SCWalletType::Biz,
        )),
        // TWT token.
        to_address: "0x4B0F1812e5Df2A09796481Ff14017e6005508003".into(),
        ..Proto::SigningInput::default()
    };

    let output = Signer::<StandardEvmContext>::sign_proto(input);
    assert_eq!(
        output.error,
        SigningErrorType::OK,
        "{}",
        output.error_message
    );

    assert_eq!(
        output.pre_hash.to_hex(),
        "60260356568ae70838bd80085b971e1e4ebe42046688fd8511a268986e522121"
    );
    // Successfully broadcasted transaction:
    // https://bscscan.com/tx/0x6f8b2c8d50e8bb543d7124703b75d9e495832116a1a61afabf40b9b0ac43c980
    assert_eq!(
        output.encoded.to_hex(),
        "02f901503802843b9aca00843b9aca00830186a0946e860086bba8fdeafb553815af0f09a854cc887a80b8e4b61d27f60000000000000000000000004b0f1812e5df2a09796481ff14017e6005508003000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044a9059cbb00000000000000000000000095dc01ebd10b6dccf1cc329af1a3f73806117c2e0000000000000000000000000000000000000000000000000000b5e620f4800000000000000000000000000000000000000000000000000000000000c080a0fb45762a262f4c32090576e9de087482d25cd00b6ea2522eb7d5a40f435acdbaa0151dbd48a4f4bf06080313775fe32ececd68869d721518a92bf292e4a84322f9"
    );
}

#[test]
fn test_biz_eip1559_transfer_with_incorrect_wallet_type_error() {
    // 0x6E860086BbA8fdEafB553815aF0F09a854cC887a
    let private_key =
        hex::decode("0xe762e91cc4889a9fce79b2d2ffc079f86c48331f57b2cd16a33bee060fe448e1").unwrap();

    let erc20_transfer = Proto::mod_Transaction::ERC20Transfer {
        to: "0x95dc01ebd10b6dccf1cc329af1a3f73806117c2e".into(),
        amount: U256::encode_be_compact(200_000_000_000_000_u64),
    };
    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(56_u64),
        nonce: U256::encode_be_compact(2_u64),
        tx_mode: Proto::TransactionMode::Enveloped,
        gas_limit: U256::from(100_000_u128).to_big_endian_compact().into(),
        max_fee_per_gas: U256::from(1_000_000_000_u128)
            .to_big_endian_compact()
            .into(),
        max_inclusion_fee_per_gas: U256::from(1_000_000_000_u128)
            .to_big_endian_compact()
            .into(),
        private_key: private_key.into(),
        transaction: Some(execute(
            TransactionType::erc20_transfer(erc20_transfer),
            // Biz4337 account cannot be used in Legacy/Enveloped/SetCode transaction flow.
            Proto::SCWalletType::Biz4337,
        )),
        // TWT token.
        to_address: "0x4B0F1812e5Df2A09796481Ff14017e6005508003".into(),
        ..Proto::SigningInput::default()
    };

    let output = Signer::<StandardEvmContext>::sign_proto(input);
    assert_eq!(output.error, SigningErrorType::Error_invalid_params,);
}

#[test]
fn test_user_operation_transfer_with_incorrect_wallet_type_error() {
    let private_key =
        hex::decode("0x3c90badc15c4d35733769093d3733501e92e7f16e101df284cee9a310d36c483").unwrap();

    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(0x23_86f2_6fc1_0000),
        data: Cow::default(),
    };
    let user_op = Proto::UserOperationV0_7 {
        entry_point: "0x0000000071727De22E5E9d8BAf0edAc6f37da032".into(),
        sender: "0x174a240e5147D02dE4d7724D5D3E1c1bF11cE029".into(),
        pre_verification_gas: U256::from(1000000u64).to_big_endian_compact().into(),
        verification_gas_limit: U256::from(100000u128).to_big_endian_compact().into(),
        factory: "0xf471789937856d80e589f5996cf8b0511ddd9de4".into(),
        factory_data: "f471789937856d80e589f5996cf8b0511ddd9de4".decode_hex().unwrap().into(),
        paymaster: "0xf62849f9a0b5bf2913b396098f7c7019b51a820a".into(),
        paymaster_verification_gas_limit: U256::from(99999u128).to_big_endian_compact().into(),
        paymaster_post_op_gas_limit: U256::from(88888u128).to_big_endian_compact().into(),
        paymaster_data: "00000000000b0000000000002e234dae75c793f67a35089c9d99245e1c58470b00000000000000000000000000000000000000000000000000000000000186a0072f35038bcacc31bcdeda87c1d9857703a26fb70a053f6e87da5a4e7a1e1f3c4b09fbe2dbff98e7a87ebb45a635234f4b79eff3225d07560039c7764291c97e1b".decode_hex().unwrap().into(),
    };

    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(31337u64),
        nonce: U256::encode_be_compact(0u64),
        tx_mode: Proto::TransactionMode::UserOp,
        gas_limit: U256::from(100000u128).to_big_endian_compact().into(),
        max_fee_per_gas: U256::from(100000u128).to_big_endian_compact().into(),
        max_inclusion_fee_per_gas: U256::from(100000u128).to_big_endian_compact().into(),
        to_address: "0x61061fCAE11fD5461535e134EfF67A98CFFF44E9".into(),
        private_key: private_key.into(),
        transaction: Some(execute(
            TransactionType::transfer(transfer),
            // Biz account cannot be used in UserOperation flow.
            Proto::SCWalletType::Biz,
        )),
        user_operation_oneof:
            Proto::mod_SigningInput::OneOfuser_operation_oneof::user_operation_v0_7(user_op),
        ..Proto::SigningInput::default()
    };

    let output = Signer::<StandardEvmContext>::sign_proto(input);
    assert_eq!(output.error, SigningErrorType::Error_invalid_params);
}
