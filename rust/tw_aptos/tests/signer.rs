// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::str::FromStr;
use move_core_types::account_address::AccountAddress;
use move_core_types::language_storage::TypeTag;
use tw_aptos::liquid_staking::LiquidStakingOperation;
use tw_aptos::nft::{Claim, NftOperation, Offer};
use tw_aptos::signer::{Signer, StandardAptosContext};
use tw_aptos::transaction_payload::convert_type_tag_to_struct_tag;
use tw_coin_entry::error::SigningErrorType;
use tw_encoding::hex;
use tw_proto::Aptos::Proto;
use tw_proto::Aptos::Proto::{SigningInput, SigningOutput};


pub struct AccountCreation {
    to: String,
}

pub struct Transfer {
    to: String,
    amount: u64,
}

pub struct TokenTransfer {
    transfer: Transfer,
    tag: TypeTag,
}

pub struct RegisterToken {
    coin_type: TypeTag,
}

pub enum OpsDetails {
    RegisterToken(RegisterToken),
    LiquidStakingOps(LiquidStakingOperation),
    AccountCreation(AccountCreation),
    Transfer(Transfer),
    TokenTransfer(TokenTransfer),
    ImplicitTokenTransfer(TokenTransfer),
    NftOps(NftOperation),
}

fn setup_proto_transaction<'a>(sender: &'a str,
                               keypair_str: &'a str,
                               transaction_type: &'a str,
                               sequence_number: i64,
                               chain_id: u32,
                               max_gas_amount: u64,
                               timestamp: u64,
                               gas_unit_price: u64,
                               any_encoded: &'a str,
                               ops_details: Option<OpsDetails>) -> SigningInput<'a> {
    let private = hex::decode(keypair_str).unwrap();

    let payload: Proto::mod_SigningInput::OneOftransaction_payload = match transaction_type {
        "transfer" => {
            if let OpsDetails::Transfer(transfer) = ops_details.unwrap() {
                Proto::mod_SigningInput::OneOftransaction_payload::transfer(Proto::TransferMessage {
                    to: transfer.to.into(),
                    amount: transfer.amount,
                })
            } else {
                panic!("Unsupported arguments")
            }
        }
        "create_account" => {
            if let OpsDetails::AccountCreation(account) = ops_details.unwrap() {
                Proto::mod_SigningInput::OneOftransaction_payload::create_account(Proto::CreateAccountMessage {
                    auth_key: account.to.into(),
                })
            } else {
                panic!("Unsupported arguments")
            }
        }
        "coin_transfer" => {
            if let OpsDetails::TokenTransfer(token_transfer) = ops_details.unwrap() {
                Proto::mod_SigningInput::OneOftransaction_payload::token_transfer(Proto::TokenTransferMessage {
                    to: token_transfer.transfer.to.into(),
                    amount: token_transfer.transfer.amount,
                    function: Some(convert_type_tag_to_struct_tag(token_transfer.tag)),
                })
            } else {
                panic!("Unsupported arguments")
            }
        }
        "implicit_coin_transfer" => {
            if let OpsDetails::ImplicitTokenTransfer(token_transfer) = ops_details.unwrap() {
                Proto::mod_SigningInput::OneOftransaction_payload::token_transfer_coins(Proto::TokenTransferCoinsMessage {
                    to: token_transfer.transfer.to.into(),
                    amount: token_transfer.transfer.amount,
                    function: Some(convert_type_tag_to_struct_tag(token_transfer.tag)),
                })
            } else {
                panic!("Unsupported arguments")
            }
        }
        "nft_ops" => {
            if let OpsDetails::NftOps(nft) = ops_details.unwrap() {
                Proto::mod_SigningInput::OneOftransaction_payload::nft_message(nft.into())
            } else {
                panic!("Unsupported arguments")
            }
        }
        _ => { todo!() }
    };


    let input = SigningInput {
        chain_id,
        sender: sender.into(),
        sequence_number,
        max_gas_amount,
        gas_unit_price,
        expiration_timestamp_secs: timestamp,
        private_key: private.into(),
        any_encoded: any_encoded.into(),
        transaction_payload: payload,
    };

    input
}

fn test_tx_result(
    output: SigningOutput,
    expected_raw_txn_bytes_str: &str,
    expected_signature_str: &str,
    expected_encoded_txn_str: &str,
    json_literal: &str) {
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());


    assert_eq!(hex::encode(output.raw_txn.to_vec(), false), expected_raw_txn_bytes_str);
    assert_eq!(hex::encode(output.authenticator.unwrap().signature.to_vec(), false), expected_signature_str);
    assert_eq!(hex::encode(output.encoded.to_vec(), false), expected_encoded_txn_str);

    let json_value_expected: serde_json::Value = serde_json::from_str(json_literal).unwrap();
    let json_value: serde_json::Value = serde_json::from_str(output.json.as_ref()).unwrap();
    assert_eq!(json_value, json_value_expected);
}

#[test]
fn test_aptos_sign_transaction_transfer() {
    let input = setup_proto_transaction("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                                        "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec",
                                        "transfer",
                                        99,
                                        33,
                                        3296766,
                                        3664390082,
                                        100,
                                        "",
                                        Some(OpsDetails::Transfer(Transfer {
                                            to: "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30".to_string(),
                                            amount: 1000,
                                        })));
    let output = Signer::<StandardAptosContext>::sign_proto(input);
    test_tx_result(output,
                   "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000021",
                   "5707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01",
                   "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c405707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01",
                   r#"{
            "expiration_timestamp_secs": "3664390082",
            "gas_unit_price": "100",
            "max_gas_amount": "3296766",
            "payload": {
                "arguments": ["0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30","1000"],
                "function": "0x1::aptos_account::transfer",
                "type": "entry_function_payload",
                "type_arguments": []
            },
            "sender": "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
            "sequence_number": "99",
            "signature": {
                "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                "signature": "0x5707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01",
                "type": "ed25519_signature"
            }
        }"#);
}

#[test]
fn test_aptos_sign_create_account() {
    let input = setup_proto_transaction("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30", // Sender's address
                                        "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec", // Keypair
                                        "create_account",
                                        0, // Sequence number
                                        33,
                                        3296766,
                                        3664390082,
                                        100,
                                        "",
                                        Some(OpsDetails::AccountCreation(AccountCreation {
                                            to: "0x3aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30e".to_string(),
                                        })));
    let output = Signer::<StandardAptosContext>::sign_proto(input);
    test_tx_result(output,
                   "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3000000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e740e6372656174655f6163636f756e740001203aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30efe4d3200000000006400000000000000c2276ada0000000021", // Expected raw transaction bytes
                   "fcba3dfbec76721454ef414955f09f159660a13886b4edd8c579e3c779c29073afe7b25efa3fef9b21c2efb1cf16b4247fc0e5c8f63fdcd1c8d87f5d59f44501", // Expected signature
                   "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3000000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e740e6372656174655f6163636f756e740001203aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30efe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c40fcba3dfbec76721454ef414955f09f159660a13886b4edd8c579e3c779c29073afe7b25efa3fef9b21c2efb1cf16b4247fc0e5c8f63fdcd1c8d87f5d59f44501", // Expected encoded transaction
                   r#"{
            "expiration_timestamp_secs": "3664390082",
            "gas_unit_price": "100",
            "max_gas_amount": "3296766",
            "payload": {
                "arguments": ["0x3aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30e"],
                "function": "0x1::aptos_account::create_account",
                "type": "entry_function_payload",
                "type_arguments": []
            },
            "sender": "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
            "sequence_number": "0",
            "signature": {
                "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                "signature": "0xfcba3dfbec76721454ef414955f09f159660a13886b4edd8c579e3c779c29073afe7b25efa3fef9b21c2efb1cf16b4247fc0e5c8f63fdcd1c8d87f5d59f44501",
                "type": "ed25519_signature"
            }
        }"#);
}

#[test]
fn test_aptos_sign_coin_transfer() {
    let input = setup_proto_transaction("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30", // Sender's address
                                        "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec", // Keypair
                                        "coin_transfer",
                                        24, // Sequence number
                                        32,
                                        3296766,
                                        3664390082,
                                        100,
                                        "",
                                        Some(OpsDetails::TokenTransfer(TokenTransfer { transfer: Transfer { to: "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30".to_string(), amount: 100000 }, tag: TypeTag::from_str("0x43417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b9::coins::BTC").unwrap() })),
    );
    let output = Signer::<StandardAptosContext>::sign_proto(input);
    test_tx_result(output,
                   "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30180000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010743417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b905636f696e730342544300022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008a086010000000000fe4d3200000000006400000000000000c2276ada0000000020", // Expected raw transaction bytes
                   "7643ec8aae6198bd13ca6ea2962265859cba5a228e7d181131f6c022700dd02a7a04dc0345ad99a0289e5ab80b130b3864e6404079980bc226f1a13aee7d280a", // Expected signature
                   "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30180000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010743417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b905636f696e730342544300022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008a086010000000000fe4d3200000000006400000000000000c2276ada00000000200020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c407643ec8aae6198bd13ca6ea2962265859cba5a228e7d181131f6c022700dd02a7a04dc0345ad99a0289e5ab80b130b3864e6404079980bc226f1a13aee7d280a", // Expected encoded transaction
                   r#"{
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": ["0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30","100000"],
                        "function": "0x1::coin::transfer",
                        "type": "entry_function_payload",
                        "type_arguments": ["0x43417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b9::coins::BTC"]
                    },
                    "sender": "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "24",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0x7643ec8aae6198bd13ca6ea2962265859cba5a228e7d181131f6c022700dd02a7a04dc0345ad99a0289e5ab80b130b3864e6404079980bc226f1a13aee7d280a",
                        "type": "ed25519_signature"
                    }
                }"#);
}

#[test]
fn test_implicit_aptos_sign_coin_transfer() {
    let input = setup_proto_transaction("0x1869b853768f0ba935d67f837a66b172dd39a60ca2315f8d4e0e669bbd35cf25", // Sender's address
                                        "e7f56c77189e03699a75d8ec5c090e41f3d9d4783bc49c33df8a93d915e10de8", // Keypair
                                        "implicit_coin_transfer",
                                        2, // Sequence number
                                        1,
                                        2000,
                                        3664390082,
                                        100,
                                        "",
                                        Some(OpsDetails::ImplicitTokenTransfer(TokenTransfer { transfer: Transfer { to: "0xb7c7d12080209e9dc14498c80200706e760363fb31782247e82cf57d1d6e5d6c".to_string(), amount: 10000 }, tag: TypeTag::from_str("0xe9c192ff55cffab3963c695cff6dbf9dad6aff2bb5ac19a6415cad26a81860d9::mee_coin::MeeCoin").unwrap() })),
    );
    let output = Signer::<StandardAptosContext>::sign_proto(input);
    test_tx_result(output,
                   "1869b853768f0ba935d67f837a66b172dd39a60ca2315f8d4e0e669bbd35cf2502000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e740e7472616e736665725f636f696e730107e9c192ff55cffab3963c695cff6dbf9dad6aff2bb5ac19a6415cad26a81860d9086d65655f636f696e074d6565436f696e000220b7c7d12080209e9dc14498c80200706e760363fb31782247e82cf57d1d6e5d6c081027000000000000d0070000000000006400000000000000c2276ada0000000001", // Expected raw transaction bytes
                   "30ebd7e95cb464677f411868e2cbfcb22bc01cc63cded36c459dff45e6d2f1354ae4e090e7dfbb509851c0368b343e0e5ecaf6b08e7c1b94c186530b0f7dee0d", // Expected signature
                   "1869b853768f0ba935d67f837a66b172dd39a60ca2315f8d4e0e669bbd35cf2502000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e740e7472616e736665725f636f696e730107e9c192ff55cffab3963c695cff6dbf9dad6aff2bb5ac19a6415cad26a81860d9086d65655f636f696e074d6565436f696e000220b7c7d12080209e9dc14498c80200706e760363fb31782247e82cf57d1d6e5d6c081027000000000000d0070000000000006400000000000000c2276ada0000000001002062e7a6a486553b56a53e89dfae3f780693e537e5b0a7ed33290780e581ca83694030ebd7e95cb464677f411868e2cbfcb22bc01cc63cded36c459dff45e6d2f1354ae4e090e7dfbb509851c0368b343e0e5ecaf6b08e7c1b94c186530b0f7dee0d", // Expected encoded transaction
                   r#"{
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "2000",
                    "payload": {
                        "arguments": ["0xb7c7d12080209e9dc14498c80200706e760363fb31782247e82cf57d1d6e5d6c","10000"],
                        "function": "0x1::aptos_account::transfer_coins",
                        "type": "entry_function_payload",
                        "type_arguments": ["0xe9c192ff55cffab3963c695cff6dbf9dad6aff2bb5ac19a6415cad26a81860d9::mee_coin::MeeCoin"]
                    },
                    "sender": "0x1869b853768f0ba935d67f837a66b172dd39a60ca2315f8d4e0e669bbd35cf25",
                    "sequence_number": "2",
                    "signature": {
                        "public_key": "0x62e7a6a486553b56a53e89dfae3f780693e537e5b0a7ed33290780e581ca8369",
                        "signature": "0x30ebd7e95cb464677f411868e2cbfcb22bc01cc63cded36c459dff45e6d2f1354ae4e090e7dfbb509851c0368b343e0e5ecaf6b08e7c1b94c186530b0f7dee0d",
                        "type": "ed25519_signature"
                    }
                }"#);
}

#[test]
fn test_aptos_nft_offer() {
    let input = setup_proto_transaction("0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee", // Sender's address
                                        "7bebb6d543d17f6fe4e685cfab239fa37896edd594ff859f1df32f244fb707e2", // Keypair
                                        "nft_ops",
                                        1, // Sequence number
                                        2,
                                        3296766,
                                        3664390082,
                                        100,
                                        "",
                                        Some(OpsDetails::NftOps(NftOperation::Offer(Offer {
                                            receiver: AccountAddress::from_str("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30").unwrap(),
                                            creator: AccountAddress::from_str("0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac").unwrap(),
                                            collection: "Topaz Troopers".as_bytes().to_vec(),
                                            name: "Topaz Trooper #20068".as_bytes().to_vec(),
                                            property_version: 0,
                                            amount: 1,
                                        }))),
    );
    let output = Signer::<StandardAptosContext>::sign_proto(input);
    test_tx_result(output,
                   "783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee01000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c6f666665725f73637269707400062007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000080100000000000000fe4d3200000000006400000000000000c2276ada0000000002", // Expected raw transaction bytes
                   "af5c7357a83c69e3f425beb23eaf232f8bb36dea3b7cad4a7ab8d735cee999c8ec5285005adf69dc85a6c34b042dd0308fe92b76dad5d6ac88c7b9259902c10f", // Expected signature
                   "783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee01000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c6f666665725f73637269707400062007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000080100000000000000fe4d3200000000006400000000000000c2276ada00000000020020d1d99b67e37b483161a0fa369c46f34a3be4863c20e20fc7cdc669c0826a411340af5c7357a83c69e3f425beb23eaf232f8bb36dea3b7cad4a7ab8d735cee999c8ec5285005adf69dc85a6c34b042dd0308fe92b76dad5d6ac88c7b9259902c10f", // Expected encoded transaction
                   r#"{
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": [
                                      "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                                      "0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac",
                                      "Topaz Troopers", "Topaz Trooper #20068", "0", "1"],
                        "function": "0x3::token_transfers::offer_script",
                        "type": "entry_function_payload",
                        "type_arguments": []
                    },
                    "sender": "0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee",
                    "sequence_number": "1",
                    "signature": {
                        "public_key": "0xd1d99b67e37b483161a0fa369c46f34a3be4863c20e20fc7cdc669c0826a4113",
                        "signature": "0xaf5c7357a83c69e3f425beb23eaf232f8bb36dea3b7cad4a7ab8d735cee999c8ec5285005adf69dc85a6c34b042dd0308fe92b76dad5d6ac88c7b9259902c10f",
                        "type": "ed25519_signature"
                    }
                }"#);
}

#[test]
fn test_aptos_cancel_nft_offer() {
    let input = setup_proto_transaction("0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30", // Sender's address
                                        "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec", // Keypair
                                        "nft_ops",
                                        21, // Sequence number
                                        2,
                                        3296766,
                                        3664390082,
                                        100,
                                        "",
                                        Some(OpsDetails::NftOps(NftOperation::Cancel(Offer {
                                            receiver: AccountAddress::from_str("0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee").unwrap(),
                                            creator: AccountAddress::from_str("0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac").unwrap(),
                                            collection: "Topaz Troopers".as_bytes().to_vec(),
                                            name: "Topaz Trooper #20068".as_bytes().to_vec(),
                                            property_version: 0,
                                            amount: 0,
                                        }))),
    );
    let output = Signer::<StandardAptosContext>::sign_proto(input);
    test_tx_result(output,
                   "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3015000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572731363616e63656c5f6f666665725f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada0000000002", // Expected raw transaction bytes
                   "826722d374e276f618123e77da3ac024c89a3f97db9e09e19aa8ed06c3cdfc57d4a21c7890137f9a7c0447cc303447ba10ca5b1908e889071e0a68f48c0f260a", // Expected signature
                   "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3015000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572731363616e63656c5f6f666665725f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada00000000020020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c40826722d374e276f618123e77da3ac024c89a3f97db9e09e19aa8ed06c3cdfc57d4a21c7890137f9a7c0447cc303447ba10ca5b1908e889071e0a68f48c0f260a", // Expected encoded transaction
                   r#"{
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": [
                                      "0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee",
                                      "0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac",
                                      "Topaz Troopers", "Topaz Trooper #20068", "0"],
                        "function": "0x3::token_transfers::cancel_offer_script",
                        "type": "entry_function_payload",
                        "type_arguments": []
                    },
                    "sender": "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "21",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0x826722d374e276f618123e77da3ac024c89a3f97db9e09e19aa8ed06c3cdfc57d4a21c7890137f9a7c0447cc303447ba10ca5b1908e889071e0a68f48c0f260a",
                        "type": "ed25519_signature"
                    }
                }"#);
}

#[test]
fn test_aptos_nft_claim() {
    let input = setup_proto_transaction("0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30", // Sender's address
                                        "5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec", // Keypair
                                        "nft_ops",
                                        19, // Sequence number
                                        2,
                                        3296766,
                                        3664390082,
                                        100,
                                        "",
                                        Some(OpsDetails::NftOps(NftOperation::Claim(Claim {
                                            sender: AccountAddress::from_str("0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee").unwrap(),
                                            creator: AccountAddress::from_str("0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac").unwrap(),
                                            collection: "Topaz Troopers".as_bytes().to_vec(),
                                            name: "Topaz Trooper #20068".as_bytes().to_vec(),
                                            property_version: 0,
                                        }))),
    );
    let output = Signer::<StandardAptosContext>::sign_proto(input);
    test_tx_result(output,
                   "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3013000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c636c61696d5f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada0000000002", // Expected raw transaction bytes
                   "ede1ffb5f8f663741c2ca9597af44af81c98f7a910261bb4125f758fd0c0ebbf5bacb34f1196ad45153177729eb6d478676b364ab747da17602713f65ca2dd0a", // Expected signature
                   "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3013000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c636c61696d5f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada00000000020020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c40ede1ffb5f8f663741c2ca9597af44af81c98f7a910261bb4125f758fd0c0ebbf5bacb34f1196ad45153177729eb6d478676b364ab747da17602713f65ca2dd0a", // Expected encoded transaction
                   r#"{
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": [
                                      "0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee",
                                      "0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac",
                                      "Topaz Troopers", "Topaz Trooper #20068", "0"],
                        "function": "0x3::token_transfers::claim_script",
                        "type": "entry_function_payload",
                        "type_arguments": []
                    },
                    "sender": "0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "19",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0xede1ffb5f8f663741c2ca9597af44af81c98f7a910261bb4125f758fd0c0ebbf5bacb34f1196ad45153177729eb6d478676b364ab747da17602713f65ca2dd0a",
                        "type": "ed25519_signature"
                    }
                }"#);
}