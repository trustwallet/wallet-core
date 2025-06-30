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
use tw_evm::modules::barz::core::*;
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

use tw_proto::Barz::Proto::{ContractAddressInput, DiamondCutInput, FacetCut, FacetCutAction};

#[test]
fn test_get_counterfactual_address() {
    let input = ContractAddressInput {
        account_facet: "0x3322C04EAe11B9b14c6c289f2668b6f07071b591".into(),
        verification_facet: "0x90A6fE0A938B0d4188e9013C99A0d7D9ca6bFB63".into(),
        entry_point: "0x5FF137D4b0FDCD49DcA30c7CF57E578a026d2789".into(),
        facet_registry: "0xFd1A8170c12747060324D9079a386BD4290e6f93".into(),
        default_fallback: "0x22eB0720d9Fc4bC90BB812B309e939880B71c20d".into(),
        public_key: "0xB5547FBdC56DCE45e1B8ef75569916D438e09c46".into(),
        bytecode: "0x608060405260405162003cc638038062003cc68339818101604052810190620000299190620019ad565b60008673ffffffffffffffffffffffffffffffffffffffff16633253960f6040518163ffffffff1660e01b81526004016020604051808303816000875af115801562000079573d6000803e3d6000fd5b505050506040513d601f19601f820116820180604052508101906200009f919062001aec565b9050600060e01b817bffffffffffffffffffffffffffffffffffffffffffffffffffffffff191603620000fe576040517f5a5b4d3900000000000000000000000000000000000000000000000000000000815260040160405180910390fd5b6000600267ffffffffffffffff8111156200011e576200011d6200196b565b5b6040519080825280602002602001820160405280156200015b57816020015b62000147620018ff565b8152602001906001900390816200013d5790505b5090506000600167ffffffffffffffff8111156200017e576200017d6200196b565b5b604051908082528060200260200182016040528015620001ad5781602001602082028036833780820191505090505b5090506000600167ffffffffffffffff811115620001d057620001cf6200196b565b5b604051908082528060200260200182016040528015620001ff5781602001602082028036833780820191505090505b509050631f931c1c60e01b8260008151811062000221576200022062001b21565b5b60200260200101907bffffffffffffffffffffffffffffffffffffffffffffffffffffffff191690817bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19168152505060405180606001604052808d73ffffffffffffffffffffffffffffffffffffffff16815260200160006002811115620002ab57620002aa62001b37565b5b81526020018381525083600081518110620002cb57620002ca62001b21565b5b60200260200101819052508381600081518110620002ee57620002ed62001b21565b5b60200260200101907bffffffffffffffffffffffffffffffffffffffffffffffffffffffff191690817bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19168152505060405180606001604052808b73ffffffffffffffffffffffffffffffffffffffff1681526020016000600281111562000378576200037762001b37565b5b8152602001828152508360018151811062000398576200039762001b21565b5b6020026020010181905250620003b9846200047e60201b620001671760201c565b6200046c838c8b8e8e8d8d8d8d604051602401620003de979695949392919062001b8c565b6040516020818303038152906040527f95a21aec000000000000000000000000000000000000000000000000000000007bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19166020820180517bffffffffffffffffffffffffffffffffffffffffffffffffffffffff8381831617835250505050620004b060201b620001911760201c565b5050505050505050505050506200211f565b806200048f6200073460201b60201c565b60010160016101000a81548163ffffffff021916908360e01c021790555050565b60005b8351811015620006df576000848281518110620004d557620004d462001b21565b5b602002602001015160200151905060006002811115620004fa57620004f962001b37565b5b81600281111562000510576200050f62001b37565b5b0362000570576200056a85838151811062000530576200052f62001b21565b5b60200260200101516000015186848151811062000552576200055162001b21565b5b6020026020010151604001516200073960201b60201c565b620006c8565b6001600281111562000587576200058662001b37565b5b8160028111156200059d576200059c62001b37565b5b03620005fd57620005f7858381518110620005bd57620005bc62001b21565b5b602002602001015160000151868481518110620005df57620005de62001b21565b5b602002602001015160400151620009db60201b60201c565b620006c7565b60028081111562000613576200061262001b37565b5b81600281111562000629576200062862001b37565b5b0362000689576200068385838151811062000649576200064862001b21565b5b6020026020010151600001518684815181106200066b576200066a62001b21565b5b60200260200101516040015162000c8f60201b60201c565b620006c6565b6040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401620006bd9062001be7565b60405180910390fd5b5b5b508080620006d69062001c61565b915050620004b3565b507f8faa70878671ccd212d20771b795c50af8fd3ff6cf27f4bde57e5d4de0aeb673838383604051620007159392919062001c82565b60405180910390a16200072f828262000e3760201b60201c565b505050565b600090565b600081511162000780576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401620007779062001d97565b60405180910390fd5b60006200079262000f6b60201b60201c565b9050600073ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff160362000806576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401620007fd9062001dfb565b60405180910390fd5b60008160010160008573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206000018054905090506000816bffffffffffffffffffffffff16036200087c576200087b828562000f9860201b60201c565b5b60005b8351811015620009d4576000848281518110620008a157620008a062001b21565b5b602002602001015190506000846000016000837bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff169050600073ffffffffffffffffffffffffffffffffffffffff168173ffffffffffffffffffffffffffffffffffffffff161462000998576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004016200098f9062001e5f565b60405180910390fd5b620009ac8583868a6200107c60201b60201c565b8380620009b99062001ec3565b94505050508080620009cb9062001c61565b9150506200087f565b5050505050565b600081511162000a22576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040162000a199062001d97565b60405180910390fd5b600062000a3462000f6b60201b60201c565b9050600073ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff160362000aa8576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040162000a9f9062001dfb565b60405180910390fd5b60008160010160008573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206000018054905090506000816bffffffffffffffffffffffff160362000b1e5762000b1d828562000f9860201b60201c565b5b60005b835181101562000c8857600084828151811062000b435762000b4262001b21565b5b602002602001015190506000846000016000837bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff1690508673ffffffffffffffffffffffffffffffffffffffff168173ffffffffffffffffffffffffffffffffffffffff160362000c39576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040162000c309062001eef565b60405180910390fd5b62000c4c8582846200122960201b60201c565b62000c608583868a6200107c60201b60201c565b838062000c6d9062001ec3565b9450505050808062000c7f9062001c61565b91505062000b21565b5050505050565b600081511162000cd6576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040162000ccd9062001d97565b60405180910390fd5b600062000ce862000f6b60201b60201c565b9050600073ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff161462000d5c576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040162000d539062001f53565b60405180910390fd5b60005b825181101562000e3157600083828151811062000d815762000d8062001b21565b5b602002602001015190506000836000016000837bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff16905062000e198482846200122960201b60201c565b5050808062000e289062001c61565b91505062000d5f565b50505050565b600073ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff16031562000f675762000e988260405180606001604052806028815260200162003c7a60289139620018aa60201b60201c565b6000808373ffffffffffffffffffffffffffffffffffffffff168360405162000ec2919062001fb7565b600060405180830381855af49150503d806000811462000eff576040519150601f19603f3d011682016040523d82523d6000602084013e62000f04565b606091505b50915091508162000f645760008151111562000f235780518082602001fd5b83836040517f192105d700000000000000000000000000000000000000000000000000000000815260040162000f5b92919062001fd7565b60405180910390fd5b50505b5050565b6000807f183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd491f90508091505090565b62000fc38160405180606001604052806024815260200162003ca260249139620018aa60201b60201c565b81600201805490508260010160008373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206001018190555081600201819080600181540180825580915050600190039060005260206000200160009091909190916101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff1602179055505050565b81846000016000857bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160146101000a8154816bffffffffffffffffffffffff02191690836bffffffffffffffffffffffff1602179055508360010160008273ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206000018390806001815401808255809150506001900390600052602060002090600891828204019190066004029091909190916101000a81548163ffffffff021916908360e01c021790555080846000016000857bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160006101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff16021790555050505050565b600073ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff16036200129b576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401620012929062002003565b60405180910390fd5b3073ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff16036200130c576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401620013039062002067565b60405180910390fd5b6000836000016000837bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160149054906101000a90046bffffffffffffffffffffffff166bffffffffffffffffffffffff169050600060018560010160008673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060000180549050620013e59190620020cb565b9050808214620015805760008560010160008673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060000182815481106200144a576200144962001b21565b5b90600052602060002090600891828204019190066004029054906101000a900460e01b9050808660010160008773ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206000018481548110620014c957620014c862001b21565b5b90600052602060002090600891828204019190066004026101000a81548163ffffffff021916908360e01c021790555082866000016000837bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160146101000a8154816bffffffffffffffffffffffff02191690836bffffffffffffffffffffffff160217905550505b8460010160008573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001908152602001600020600001805480620015d757620015d6620020ec565b5b60019003818190600052602060002090600891828204019190066004026101000a81549063ffffffff02191690559055846000016000847bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19168152602001908152602001600020600080820160006101000a81549073ffffffffffffffffffffffffffffffffffffffff02191690556000820160146101000a8154906bffffffffffffffffffffffff0219169055505060008103620018a357600060018660020180549050620016c49190620020cb565b905060008660010160008773ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206001015490508181146200180c57600087600201838154811062001732576200173162001b21565b5b9060005260206000200160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff1690508088600201838154811062001779576200177862001b21565b5b9060005260206000200160006101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff160217905550818860010160008373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060010181905550505b86600201805480620018235762001822620020ec565b5b6001900381819060005260206000200160006101000a81549073ffffffffffffffffffffffffffffffffffffffff021916905590558660010160008773ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206001016000905550505b5050505050565b6000823b9050600081118290620018f9576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401620018f0919062002102565b60405180910390fd5b50505050565b6040518060600160405280600073ffffffffffffffffffffffffffffffffffffffff168152602001600060028111156200193e576200193d62001b37565b5b8152602001606081525090565b60008151905060018060a01b03811681146200196657600080fd5b919050565b634e487b7160e01b600052604160045260246000fd5b60005b83811015620019a157808201518184015260208101905062001984565b50600083830152505050565b600080600080600080600080610100898b031215620019cb57600080fd5b620019d6896200194b565b9750620019e660208a016200194b565b9650620019f660408a016200194b565b955062001a0660608a016200194b565b945062001a1660808a016200194b565b935062001a2660a08a016200194b565b925062001a3660c08a016200194b565b915060e089015160018060401b038082111562001a5257600080fd5b818b0191508b601f83011262001a6757600080fd5b81518181111562001a7d5762001a7c6200196b565b5b601f1960405181603f83601f860116011681019150808210848311171562001aaa5762001aa96200196b565b5b816040528281528e602084870101111562001ac457600080fd5b62001ad783602083016020880162001981565b80955050505050509295985092959890939650565b60006020828403121562001aff57600080fd5b815163ffffffff60e01b8116811462001b1757600080fd5b8091505092915050565b634e487b7160e01b600052603260045260246000fd5b634e487b7160e01b600052602160045260246000fd5b60018060a01b03811682525050565b6000815180845262001b7681602086016020860162001981565b6020601f19601f83011685010191505092915050565b600060018060a01b03808a168352808916602084015280881660408401528087166060840152808616608084015280851660a08401525060e060c083015262001bd960e083018462001b5c565b905098975050505050505050565b60208152602760208201527f4c69624469616d6f6e644375743a20496e636f7272656374204661636574437560408201527f74416374696f6e0000000000000000000000000000000000000000000000000060608201526000608082019050919050565b634e487b7160e01b600052601160045260246000fd5b60008019820362001c775762001c7662001c4b565b5b600182019050919050565b60006060808301818452808751808352608092508286019150828160051b8701016020808b0160005b8481101562001d6357607f198a8503018652815188850160018060a01b038251168652848201516003811062001cf157634e487b7160e01b600052602160045260246000fd5b80868801525060408083015192508a81880152508082518083528a880191508684019350600092505b8083101562001d465763ffffffff60e01b84511682528682019150868401935060018301925062001d1a565b508096505050508282019150828601955060018101905062001cab565b505062001d738189018b62001b4d565b50868103604088015262001d88818962001b5c565b95505050505050949350505050565b60208152602b60208201527f4c69624469616d6f6e644375743a204e6f2073656c6563746f727320696e206660408201527f6163657420746f2063757400000000000000000000000000000000000000000060608201526000608082019050919050565b60208152602c60208201527f4c69624469616d6f6e644375743a204164642066616365742063616e2774206260408201527f652061646472657373283029000000000000000000000000000000000000000060608201526000608082019050919050565b60208152603560208201527f4c69624469616d6f6e644375743a2043616e2774206164642066756e6374696f60408201527f6e207468617420616c726561647920657869737473000000000000000000000060608201526000608082019050919050565b600060018060601b0380831681810362001ee25762001ee162001c4b565b5b6001810192505050919050565b60208152603860208201527f4c69624469616d6f6e644375743a2043616e2774207265706c6163652066756e60408201527f6374696f6e20776974682073616d652066756e6374696f6e000000000000000060608201526000608082019050919050565b60208152603660208201527f4c69624469616d6f6e644375743a2052656d6f7665206661636574206164647260408201527f657373206d75737420626520616464726573732830290000000000000000000060608201526000608082019050919050565b6000825162001fcb81846020870162001981565b80830191505092915050565b60018060a01b038316815260406020820152600062001ffa604083018462001b5c565b90509392505050565b60208152603760208201527f4c69624469616d6f6e644375743a2043616e27742072656d6f76652066756e6360408201527f74696f6e207468617420646f65736e277420657869737400000000000000000060608201526000608082019050919050565b60208152602e60208201527f4c69624469616d6f6e644375743a2043616e27742072656d6f766520696d6d7560408201527f7461626c652066756e6374696f6e00000000000000000000000000000000000060608201526000608082019050919050565b6000828203905081811115620020e657620020e562001c4b565b5b92915050565b634e487b7160e01b600052603160045260246000fd5b60208152600062002117602083018462001b5c565b905092915050565b611b4b806200212f6000396000f3fe60806040523661000b57005b6000807f183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd491f9050809150600082600001600080357fffffffff00000000000000000000000000000000000000000000000000000000167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff169050600073ffffffffffffffffffffffffffffffffffffffff168173ffffffffffffffffffffffffffffffffffffffff1603610141576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401610138906114d3565b60405180910390fd5b3660008037600080366000845af43d6000803e8060008114610162573d6000f35b3d6000fd5b806101706103c0565b60010160016101000a81548163ffffffff021916908360e01c021790555050565b60005b83518110156103755760008482815181106101b2576101b1611510565b5b6020026020010151602001519050600060028111156101d4576101d3611526565b5b8160028111156101e7576101e6611526565b5b036102375761023285838151811061020257610201611510565b5b60200260200101516000015186848151811061022157610220611510565b5b6020026020010151604001516103c5565b610361565b6001600281111561024b5761024a611526565b5b81600281111561025e5761025d611526565b5b036102ae576102a985838151811061027957610278611510565b5b60200260200101516000015186848151811061029857610297611510565b5b60200260200101516040015161063c565b610360565b6002808111156102c1576102c0611526565b5b8160028111156102d4576102d3611526565b5b036103245761031f8583815181106102ef576102ee611510565b5b60200260200101516000015186848151811061030e5761030d611510565b5b6020026020010151604001516108bd565b61035f565b6040517f08c379a00000000000000000000000000000000000000000000000000000000081526004016103569061153c565b60405180910390fd5b5b5b50808061036d906115b6565b915050610194565b507f8faa70878671ccd212d20771b795c50af8fd3ff6cf27f4bde57e5d4de0aeb6738383836040516103a99392919061163b565b60405180910390a16103bb8282610a48565b505050565b600090565b6000815111610409576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040161040090611747565b60405180910390fd5b6000610413610b6a565b9050600073ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff1603610484576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040161047b906117ab565b60405180910390fd5b60008160010160008573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206000018054905090506000816bffffffffffffffffffffffff16036104f1576104f08285610b97565b5b60005b835181101561063557600084828151811061051257610511611510565b5b602002602001015190506000846000016000837bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff169050600073ffffffffffffffffffffffffffffffffffffffff168173ffffffffffffffffffffffffffffffffffffffff1614610606576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004016105fd9061180f565b60405180910390fd5b6106128583868a610c72565b838061061d90611873565b9450505050808061062d906115b6565b9150506104f4565b5050505050565b6000815111610680576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040161067790611747565b60405180910390fd5b600061068a610b6a565b9050600073ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff16036106fb576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004016106f2906117ab565b60405180910390fd5b60008160010160008573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206000018054905090506000816bffffffffffffffffffffffff1603610768576107678285610b97565b5b60005b83518110156108b657600084828151811061078957610788611510565b5b602002602001015190506000846000016000837bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff1690508673ffffffffffffffffffffffffffffffffffffffff168173ffffffffffffffffffffffffffffffffffffffff160361087c576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401610873906118a2565b60405180910390fd5b610887858284610e1f565b6108938583868a610c72565b838061089e90611873565b945050505080806108ae906115b6565b91505061076b565b5050505050565b6000815111610901576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004016108f890611747565b60405180910390fd5b600061090b610b6a565b9050600073ffffffffffffffffffffffffffffffffffffffff168373ffffffffffffffffffffffffffffffffffffffff161461097c576040517f08c379a000000000000000000000000000000000000000000000000000000000815260040161097390611906565b60405180910390fd5b60005b8251811015610a4257600083828151811061099d5761099c611510565b5b602002602001015190506000836000016000837bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff169050610a2d848284610e1f565b50508080610a3a906115b6565b91505061097f565b50505050565b600073ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff160315610b6657610a9f82604051806060016040528060288152602001611aca60289139611481565b6000808373ffffffffffffffffffffffffffffffffffffffff1683604051610ac7919061196a565b600060405180830381855af49150503d8060008114610b02576040519150601f19603f3d011682016040523d82523d6000602084013e610b07565b606091505b509150915081610b6357600081511115610b245780518082602001fd5b83836040517f192105d7000000000000000000000000000000000000000000000000000000008152600401610b5a929190611988565b60405180910390fd5b50505b5050565b6000807f183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd491f90508091505090565b610bb981604051806060016040528060248152602001611af260249139611481565b81600201805490508260010160008373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206001018190555081600201819080600181540180825580915050600190039060005260206000200160009091909190916101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff1602179055505050565b81846000016000857bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160146101000a8154816bffffffffffffffffffffffff02191690836bffffffffffffffffffffffff1602179055508360010160008273ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206000018390806001815401808255809150506001900390600052602060002090600891828204019190066004029091909190916101000a81548163ffffffff021916908360e01c021790555080846000016000857bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160006101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff16021790555050505050565b600073ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff1603610e8e576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401610e85906119b2565b60405180910390fd5b3073ffffffffffffffffffffffffffffffffffffffff168273ffffffffffffffffffffffffffffffffffffffff1603610efc576040517f08c379a0000000000000000000000000000000000000000000000000000000008152600401610ef390611a16565b60405180910390fd5b6000836000016000837bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160149054906101000a90046bffffffffffffffffffffffff166bffffffffffffffffffffffff169050600060018560010160008673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060000180549050610fd39190611a7a565b90508082146111675760008560010160008673ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff168152602001908152602001600020600001828154811061103457611033611510565b5b90600052602060002090600891828204019190066004029054906101000a900460e01b9050808660010160008773ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060000184815481106110b0576110af611510565b5b90600052602060002090600891828204019190066004026101000a81548163ffffffff021916908360e01c021790555082866000016000837bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff1916815260200190815260200160002060000160146101000a8154816bffffffffffffffffffffffff02191690836bffffffffffffffffffffffff160217905550505b8460010160008573ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206000018054806111bb576111ba611a98565b5b60019003818190600052602060002090600891828204019190066004026101000a81549063ffffffff02191690559055846000016000847bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19167bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19168152602001908152602001600020600080820160006101000a81549073ffffffffffffffffffffffffffffffffffffffff02191690556000820160146101000a8154906bffffffffffffffffffffffff021916905550506000810361147a576000600186600201805490506112a59190611a7a565b905060008660010160008773ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206001015490508181146113e657600087600201838154811061130f5761130e611510565b5b9060005260206000200160009054906101000a900473ffffffffffffffffffffffffffffffffffffffff1690508088600201838154811061135357611352611510565b5b9060005260206000200160006101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff160217905550818860010160008373ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16815260200190815260200160002060010181905550505b866002018054806113fa576113f9611a98565b5b6001900381819060005260206000200160006101000a81549073ffffffffffffffffffffffffffffffffffffffff021916905590558660010160008773ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff1681526020019081526020016000206001016000905550505b5050505050565b6000823b90506000811182906114cd576040517f08c379a00000000000000000000000000000000000000000000000000000000081526004016114c49190611aae565b60405180910390fd5b50505050565b602081526020808201527f4469616d6f6e643a2046756e6374696f6e20646f6573206e6f7420657869737460408201526000606082019050919050565b634e487b7160e01b600052603260045260246000fd5b634e487b7160e01b600052602160045260246000fd5b60208152602760208201527f4c69624469616d6f6e644375743a20496e636f7272656374204661636574437560408201527f74416374696f6e0000000000000000000000000000000000000000000000000060608201526000608082019050919050565b634e487b7160e01b600052601160045260246000fd5b6000801982036115c9576115c86115a0565b5b600182019050919050565b60018060a01b03811682525050565b60005b838110156116015780820151818401526020810190506115e6565b50600083830152505050565b600081518084526116258160208601602086016115e3565b6020601f19601f83011685010191505092915050565b60006060808301818452808751808352608092508286019150828160051b8701016020808b0160005b8481101561171757607f198a8503018652815188850160018060a01b03825116865284820151600381106116a857634e487b7160e01b600052602160045260246000fd5b80868801525060408083015192508a81880152508082518083528a880191508684019350600092505b808310156116fb5763ffffffff60e01b8451168252868201915086840193506001830192506116d1565b5080965050505082820191508286019550600181019050611664565b50506117258189018b6115d4565b508681036040880152611738818961160d565b95505050505050949350505050565b60208152602b60208201527f4c69624469616d6f6e644375743a204e6f2073656c6563746f727320696e206660408201527f6163657420746f2063757400000000000000000000000000000000000000000060608201526000608082019050919050565b60208152602c60208201527f4c69624469616d6f6e644375743a204164642066616365742063616e2774206260408201527f652061646472657373283029000000000000000000000000000000000000000060608201526000608082019050919050565b60208152603560208201527f4c69624469616d6f6e644375743a2043616e2774206164642066756e6374696f60408201527f6e207468617420616c726561647920657869737473000000000000000000000060608201526000608082019050919050565b60006bffffffffffffffffffffffff808316818103611895576118946115a0565b5b6001810192505050919050565b60208152603860208201527f4c69624469616d6f6e644375743a2043616e2774207265706c6163652066756e60408201527f6374696f6e20776974682073616d652066756e6374696f6e000000000000000060608201526000608082019050919050565b60208152603660208201527f4c69624469616d6f6e644375743a2052656d6f7665206661636574206164647260408201527f657373206d75737420626520616464726573732830290000000000000000000060608201526000608082019050919050565b6000825161197c8184602087016115e3565b80830191505092915050565b60018060a01b03831681526040602082015260006119a9604083018461160d565b90509392505050565b60208152603760208201527f4c69624469616d6f6e644375743a2043616e27742072656d6f76652066756e6360408201527f74696f6e207468617420646f65736e277420657869737400000000000000000060608201526000608082019050919050565b60208152602e60208201527f4c69624469616d6f6e644375743a2043616e27742072656d6f766520696d6d7560408201527f7461626c652066756e6374696f6e00000000000000000000000000000000000060608201526000608082019050919050565b6000828203905081811115611a9257611a916115a0565b5b92915050565b634e487b7160e01b600052603160045260246000fd5b602081526000611ac1602083018461160d565b90509291505056fe4c69624469616d6f6e644375743a205f696e6974206164647265737320686173206e6f20636f64654c69624469616d6f6e644375743a204e657720666163657420686173206e6f20636f6465a264697066735822122045b771fb2128a1a34c5b052e9a86464933844b34929cf0d65bbea6a4e76e3b2764736f6c634300081200334c69624469616d6f6e644375743a205f696e6974206164647265737320686173206e6f20636f64654c69624469616d6f6e644375743a204e657720666163657420686173206e6f20636f6465".into(),
        salt: 0,
        factory: "0x2c97f4a366Dd5D91178ec9E36c5C1fcA393A538C".into(),
    };
    let address = get_counterfactual_address(&input).unwrap();
    assert_eq!(address, "0x77F62bb3E43190253D4E198199356CD2b25063cA");
}

#[test]
fn test_get_counterfactual_address_non_zero_salt() {
    let input = ContractAddressInput {
        account_facet: "0xF6F5e5fC74905e65e3FF53c6BacEba8535dd14d1".into(),
        verification_facet: "0xaB84813cbf26Fd951CB3d7E33Dccb8995027e490".into(),
        entry_point: "0x5FF137D4b0FDCD49DcA30c7CF57E578a026d2789".into(),
        facet_registry: "0x9a95d201BB8F559771784D12c01F8084278c65E5".into(),
        default_fallback: "0x522cDc7558b5f798dF5D61AB09B6D95Ebd342EF9".into(),
        public_key: "0xB5547FBdC56DCE45e1B8ef75569916D438e09c46".into(),
        bytecode: "0x60806040526040516104c83803806104c883398101604081905261002291610163565b6000858585858560405160240161003d959493929190610264565b60408051601f198184030181529181526020820180516001600160e01b0316634a93641760e01b1790525190915060009081906001600160a01b038a16906100869085906102c3565b600060405180830381855af49150503d80600081146100c1576040519150601f19603f3d011682016040523d82523d6000602084013e6100c6565b606091505b50915091508115806100e157506100dc816102df565b600114155b156100ff57604051636ff35f8960e01b815260040160405180910390fd5b505050505050505050610306565b80516001600160a01b038116811461012457600080fd5b919050565b634e487b7160e01b600052604160045260246000fd5b60005b8381101561015a578181015183820152602001610142565b50506000910152565b60008060008060008060c0878903121561017c57600080fd5b6101858761010d565b95506101936020880161010d565b94506101a16040880161010d565b93506101af6060880161010d565b92506101bd6080880161010d565b60a08801519092506001600160401b03808211156101da57600080fd5b818901915089601f8301126101ee57600080fd5b81518181111561020057610200610129565b604051601f8201601f19908116603f0116810190838211818310171561022857610228610129565b816040528281528c602084870101111561024157600080fd5b61025283602083016020880161013f565b80955050505050509295509295509295565b600060018060a01b0380881683528087166020840152808616604084015280851660608401525060a0608083015282518060a08401526102ab8160c085016020870161013f565b601f01601f19169190910160c0019695505050505050565b600082516102d581846020870161013f565b9190910192915050565b80516020808301519190811015610300576000198160200360031b1b821691505b50919050565b6101b3806103156000396000f3fe60806040523661000b57005b600080356001600160e01b03191681527f183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd491f6020819052604090912054819060601c806100cf576004838101546040516366ffd66360e11b81526000356001600160e01b031916928101929092526001600160a01b03169063cdffacc690602401602060405180830381865afa1580156100a8573d6000803e3d6000fd5b505050506040513d601f19601f820116820180604052508101906100cc919061014d565b90505b6001600160a01b0381166101295760405162461bcd60e51b815260206004820152601d60248201527f4261727a3a2046756e6374696f6e20646f6573206e6f74206578697374000000604482015260640160405180910390fd5b3660008037600080366000845af43d6000803e808015610148573d6000f35b3d6000fd5b60006020828403121561015f57600080fd5b81516001600160a01b038116811461017657600080fd5b939250505056fea2646970667358221220d35db061bb6ecdb7688c3674af669ce44d527cae4ded59214d06722d73da62be64736f6c63430008120033".into(),
        salt: 123456,
        factory: "0x96C489979E39F877BDb8637b75A25C1a5B2DE14C".into(),
    };
    let address = get_counterfactual_address(&input).unwrap();
    assert_eq!(address, "0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1");
}

#[test]
fn test_get_init_code() {
    let public_key = hex::decode("04e6f4e0351e2f556fd7284a9a033832bae046ac31fd529ad02ab6220870624b79eb760e718fdaed7a037dd1d77a561759cee9f2706eb55a729dc953e0d5719b02").unwrap();

    let factory_address = "0x3fC708630d85A3B5ec217E53100eC2b735d4f800";
    let verification_facet = "0x6BF22ff186CC97D88ECfbA47d1473a234CEBEFDf";
    let salt = 0;

    let init_code = get_init_code(factory_address, &public_key, verification_facet, salt).unwrap();
    assert_eq!(hex::encode(init_code, true), "0x3fc708630d85a3b5ec217e53100ec2b735d4f800296601cd0000000000000000000000006bf22ff186cc97d88ecfba47d1473a234cebefdf00000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004104e6f4e0351e2f556fd7284a9a033832bae046ac31fd529ad02ab6220870624b79eb760e718fdaed7a037dd1d77a561759cee9f2706eb55a729dc953e0d5719b0200000000000000000000000000000000000000000000000000000000000000");

    let salt = 1;
    let init_code = get_init_code(factory_address, &public_key, verification_facet, salt).unwrap();
    assert_eq!(hex::encode(init_code, true), "0x3fc708630d85a3b5ec217e53100ec2b735d4f800296601cd0000000000000000000000006bf22ff186cc97d88ecfba47d1473a234cebefdf00000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000004104e6f4e0351e2f556fd7284a9a033832bae046ac31fd529ad02ab6220870624b79eb760e718fdaed7a037dd1d77a561759cee9f2706eb55a729dc953e0d5719b0200000000000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_formatted_signature() {
    let signature = hex::decode("3044022012d89e3b41e253dc9e90bd34dc1750d059b76d0b1d16af2059aa26e90b8960bf0220256d8a05572c654906ce422464693e280e243e6d9dbc5f96a681dba846bca276").unwrap();
    let challenge =
        hex::decode("cf267a78c5adaf96f341a696eb576824284c572f3e61be619694d539db1925f9").unwrap();
    let authenticator_data =
        hex::decode("1a70842af8c1feb7133b81e6a160a6a2be45ee057f0eb6d3f7f5126daa202e071d00000000")
            .unwrap();
    let client_data_json = "{\"type\":\"webauthn.get\",\"challenge\":\"zyZ6eMWtr5bzQaaW61doJChMVy8-Yb5hlpTVOdsZJfk\",\"origin\":\"https://trustwallet.com\"}";

    let formatted_signature = get_formatted_signature(
        &signature,
        &challenge,
        &authenticator_data,
        client_data_json,
    )
    .unwrap();
    assert_eq!(hex::encode(formatted_signature, true), "0x12d89e3b41e253dc9e90bd34dc1750d059b76d0b1d16af2059aa26e90b8960bf256d8a05572c654906ce422464693e280e243e6d9dbc5f96a681dba846bca27600000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000016000000000000000000000000000000000000000000000000000000000000000251a70842af8c1feb7133b81e6a160a6a2be45ee057f0eb6d3f7f5126daa202e071d0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000247b2274797065223a22776562617574686e2e676574222c226368616c6c656e6765223a22000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000025222c226f726967696e223a2268747470733a2f2f747275737477616c6c65742e636f6d227d000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_prefixed_msg_hash() {
    let msg_hash =
        hex::decode("a6ebe22d8c1ec7edbd7f5776e49a161f67ab97161d7b8c648d80abf365765cf2").unwrap();
    let barz_address = "0x913233BfC283ffe89a5E70ADC39c0926d240bbD9";
    let chain_id = 3604;

    let prefixed_msg_hash = get_prefixed_msg_hash(&msg_hash, barz_address, chain_id).unwrap();
    assert_eq!(
        hex::encode(prefixed_msg_hash, true),
        "0x0488fb3e4fdaa890bf55532fc9840fb9edef9c38244f431c9430a78a86d89157"
    );
}

#[test]
fn test_get_prefixed_msg_hash_with_zero_chain_id() {
    let msg_hash =
        hex::decode("cf267a78c5adaf96f341a696eb576824284c572f3e61be619694d539db1925f9").unwrap();
    let barz_address = "0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1";
    let chain_id = 0;

    let prefixed_msg_hash = get_prefixed_msg_hash(&msg_hash, barz_address, chain_id).unwrap();
    assert_eq!(
        hex::encode(prefixed_msg_hash, true),
        "0xc74e78634261222af51530703048f98a1b7b995a606a624f0a008e7aaba7a21b"
    );
}

#[test]
fn test_get_diamond_cut_code() {
    let input = DiamondCutInput {
        init_address: "0x0000000000000000000000000000000000000000".into(),
        init_data: hex::decode("0x00").unwrap().into(),
        facet_cuts: vec![FacetCut {
            facet_address: "0x2279B7A0a67DB372996a5FaB50D91eAA73d2eBe6".into(),
            action: FacetCutAction::ADD,
            function_selectors: vec![hex::decode("fdd8a83c").unwrap().into()],
        }],
    };

    let diamond_cut_code = get_diamond_cut_code(&input).unwrap();
    assert_eq!(hex::encode(diamond_cut_code, true), "0x1f931c1c000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000140000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000002279b7a0a67db372996a5fab50d91eaa73d2ebe6000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000001fdd8a83c0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_diamond_cut_code_with_multiple_cut() {
    let input = DiamondCutInput {
        init_address: "0x2279B7A0a67DB372996a5FaB50D91eAA73d2eBe6".into(),
        init_data: hex::decode("12341234").unwrap().into(),
        facet_cuts: vec![
            FacetCut {
                facet_address: "0x2279B7A0a67DB372996a5FaB50D91eAA73d2eBe6".into(),
                action: FacetCutAction::ADD,
                function_selectors: vec![
                    hex::decode("fdd8a83c").unwrap().into(),
                    hex::decode("fdd8a83c").unwrap().into(),
                    hex::decode("fdd8a83c").unwrap().into(),
                ],
            },
            FacetCut {
                facet_address: "0x6e3c94d74af6227aEeF75b54a679e969189a6aEC".into(),
                action: FacetCutAction::ADD,
                function_selectors: vec![hex::decode("12345678").unwrap().into()],
            },
        ],
    };

    let diamond_cut_code = get_diamond_cut_code(&input).unwrap();
    assert_eq!(hex::encode(diamond_cut_code, true), "0x1f931c1c00000000000000000000000000000000000000000000000000000000000000600000000000000000000000002279b7a0a67db372996a5fab50d91eaa73d2ebe600000000000000000000000000000000000000000000000000000000000002400000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000001200000000000000000000000002279b7a0a67db372996a5fab50d91eaa73d2ebe6000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000003fdd8a83c00000000000000000000000000000000000000000000000000000000fdd8a83c00000000000000000000000000000000000000000000000000000000fdd8a83c000000000000000000000000000000000000000000000000000000000000000000000000000000006e3c94d74af6227aeef75b54a679e969189a6aec000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000001123456780000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000041234123400000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_diamond_cut_code_with_zero_selector() {
    let input = DiamondCutInput {
        init_address: "0x0000000000000000000000000000000000000000".into(),
        init_data: hex::decode("00").unwrap().into(),
        facet_cuts: vec![FacetCut {
            facet_address: "0x2279B7A0a67DB372996a5FaB50D91eAA73d2eBe6".into(),
            action: FacetCutAction::ADD,
            function_selectors: vec![],
        }],
    };

    let diamond_cut_code = get_diamond_cut_code(&input).unwrap();
    assert_eq!(hex::encode(diamond_cut_code, true), "0x1f931c1c000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000120000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000002279b7a0a67db372996a5fab50d91eaa73d2ebe600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_diamond_cut_code_with_long_init_data() {
    let input = DiamondCutInput {
        init_address: "0x0000000000000000000000000000000000000000".into(),
        init_data: hex::decode("b61d27f6000000000000000000000000c2ce171d25837cd43e496719f5355a847edc679b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000024a526d83b00000000000000000000000090f79bf6eb2c4f870365e785982e1f101e93b90600000000000000000000000000000000000000000000000000000000").unwrap().into(),
        facet_cuts: vec![
            FacetCut {
                facet_address: "0x2279B7A0a67DB372996a5FaB50D91eAA73d2eBe6".into(),
                action: FacetCutAction::ADD,
                function_selectors: vec![],
            },
        ],
    };

    let diamond_cut_code = get_diamond_cut_code(&input).unwrap();
    assert_eq!(hex::encode(diamond_cut_code, true), "0x1f931c1c000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000120000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000002279b7a0a67db372996a5fab50d91eaa73d2ebe600000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000060000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000c4b61d27f6000000000000000000000000c2ce171d25837cd43e496719f5355a847edc679b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000024a526d83b00000000000000000000000090f79bf6eb2c4f870365e785982e1f101e93b9060000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
}

#[test]
fn test_get_authorization_hash() {
    let chain_id = U256::from(1u64).to_big_endian();
    let contract_address = "0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1";
    let nonce = U256::from(1u64).to_big_endian();

    let authorization_hash =
        get_authorization_hash(&chain_id[..], contract_address, &nonce[..]).unwrap();
    assert_eq!(
        hex::encode(authorization_hash, true),
        "0x3ae543b2fa103a39a6985d964a67caed05f6b9bb2430ad6d498cda743fe911d9"
    ); // Verified with viem
}

#[test]
fn test_sign_authorization() {
    let chain_id = U256::from(1u64).to_big_endian_compact();
    let contract_address = "0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1";
    let nonce = U256::from(1u64).to_big_endian_compact();
    let private_key = "0x947dd69af402e7f48da1b845dfc1df6be593d01a0d8274bd03ec56712e7164e8";

    let signed_authorization =
        sign_authorization(&chain_id[..], contract_address, &nonce[..], private_key).unwrap();
    let json: serde_json::Value = serde_json::from_str(&signed_authorization).unwrap();

    // Verified with viem
    assert_eq!(
        json["chainId"].as_str().unwrap(),
        hex::encode(chain_id, true)
    );
    assert_eq!(json["address"].as_str().unwrap(), contract_address);
    assert_eq!(json["nonce"].as_str().unwrap(), hex::encode(nonce, true));
    assert_eq!(json["yParity"].as_str().unwrap(), hex::encode(&[1u8], true));
    assert_eq!(
        json["r"].as_str().unwrap(),
        "0x2c39f2f64441dd38c364ee175dc6b9a87f34ca330bce968f6c8e22811e3bb710"
    );
    assert_eq!(
        json["s"].as_str().unwrap(),
        "0x5f1bcde93dee4b214e60bc0e63babcc13e4fecb8a23c4098fd89844762aa012c"
    );
}

#[test]
fn test_get_encoded_hash() {
    let chain_id = U256::from(31337u64).to_big_endian();
    let code_address = "0x2e234DAe75C793f67A35089C9d99245E1C58470b";
    let code_name = "Biz";
    let code_version = "v1.0.0";
    let type_hash = "0x4f51e7a567f083a31264743067875fc6a7ae45c32c5bd71f6a998c4625b13867";
    let domain_separator_hash =
        "0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472";
    let sender = "0x174a240e5147D02dE4d7724D5D3E1c1bF11cE029";
    let user_op_hash = "0xf177858c1c500e51f38ffe937bed7e4d3a8678725900be4682d3ce04d97071eb";

    let encoded_hash = get_encoded_hash(
        &chain_id[..],
        code_address,
        code_name,
        code_version,
        type_hash,
        domain_separator_hash,
        sender,
        user_op_hash,
    )
    .unwrap();
    assert_eq!(
        hex::encode(encoded_hash, true),
        "0xc63891abc38f7a991f89ad7cb6d7e53543627b0536c3f5e545b736756c971635"
    );
}

#[test]
fn test_sign_user_op_hash() {
    let hash = "0xc63891abc38f7a991f89ad7cb6d7e53543627b0536c3f5e545b736756c971635";
    let private_key = "0x947dd69af402e7f48da1b845dfc1df6be593d01a0d8274bd03ec56712e7164e8";
    let signed_hash = sign_user_op_hash(hash, private_key).unwrap();
    assert_eq!(hex::encode(signed_hash, true), "0xa29e460720e4b539f593d1a407827d9608cccc2c18b7af7b3689094dca8a016755bca072ffe39bc62285b65aff8f271f20798a421acf18bb2a7be8dbe0eb05f81c");
}
