// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Aptos/Address.h"
#include "Aptos/Signer.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TestUtilities.h"
#include <nlohmann/json.hpp>

#include <gtest/gtest.h>

namespace TW::Aptos::tests {

TEST(AptosSigner, ClaimNftTxSign) {
    // Successfully broadcasted: https://explorer.aptoslabs.com/txn/0x60b51e15140ec0b7650334e948fb447ce3cb13ae63492260461ebfa9d02e85c4?network=testnet
    Proto::SigningInput input;
    input.set_sender("0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30");
    input.set_sequence_number(19);
    auto& tf = *input.mutable_nft_message()->mutable_claim_nft();
    tf.set_sender("0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee");
    tf.set_creator("0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac");
    tf.set_collectionname("Topaz Troopers");
    tf.set_name("Topaz Trooper #20068");
    tf.set_property_version(0);
    input.set_max_gas_amount(3296766);
    input.set_gas_unit_price(100);
    input.set_expiration_timestamp_secs(3664390082);
    input.set_chain_id(2);
    auto privateKey = PrivateKey(parse_hex("5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(hex(result.raw_txn()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3013000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c636c61696d5f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada0000000002");
    ASSERT_EQ(hex(result.authenticator().signature()), "ede1ffb5f8f663741c2ca9597af44af81c98f7a910261bb4125f758fd0c0ebbf5bacb34f1196ad45153177729eb6d478676b364ab747da17602713f65ca2dd0a");
    ASSERT_EQ(hex(result.encoded()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3013000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c636c61696d5f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada00000000020020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c40ede1ffb5f8f663741c2ca9597af44af81c98f7a910261bb4125f758fd0c0ebbf5bacb34f1196ad45153177729eb6d478676b364ab747da17602713f65ca2dd0a");
    nlohmann::json expectedJson = R"(
                {
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
                    "sender": "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "19",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0xede1ffb5f8f663741c2ca9597af44af81c98f7a910261bb4125f758fd0c0ebbf5bacb34f1196ad45153177729eb6d478676b364ab747da17602713f65ca2dd0a",
                        "type": "ed25519_signature"
                    }
                }
        )"_json;
    nlohmann::json parsedJson = nlohmann::json::parse(result.json());
    assertJSONEqual(expectedJson, parsedJson);
}

TEST(AptosSigner, NftOfferTxSign) {
    // Successfully broadcasted https://explorer.aptoslabs.com/txn/0x514e473618bd3cb89a2b110b7c473db9a2e10532f98eb42d02d86fb31c00525d?network=testnet
    Proto::SigningInput input;
    input.set_sender("0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee");
    input.set_sequence_number(1);
    auto& tf = *input.mutable_nft_message()->mutable_offer_nft();
    tf.set_receiver("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30");
    tf.set_creator("0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac");
    tf.set_collectionname("Topaz Troopers");
    tf.set_name("Topaz Trooper #20068");
    tf.set_property_version(0);
    tf.set_amount(1);
    input.set_max_gas_amount(3296766);
    input.set_gas_unit_price(100);
    input.set_expiration_timestamp_secs(3664390082);
    input.set_chain_id(2);
    auto privateKey = PrivateKey(parse_hex("7bebb6d543d17f6fe4e685cfab239fa37896edd594ff859f1df32f244fb707e2"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(hex(result.raw_txn()), "783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee01000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c6f666665725f73637269707400062007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000080100000000000000fe4d3200000000006400000000000000c2276ada0000000002");
    ASSERT_EQ(hex(result.authenticator().signature()), "af5c7357a83c69e3f425beb23eaf232f8bb36dea3b7cad4a7ab8d735cee999c8ec5285005adf69dc85a6c34b042dd0308fe92b76dad5d6ac88c7b9259902c10f");
    ASSERT_EQ(hex(result.encoded()), "783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee01000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572730c6f666665725f73637269707400062007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000080100000000000000fe4d3200000000006400000000000000c2276ada00000000020020d1d99b67e37b483161a0fa369c46f34a3be4863c20e20fc7cdc669c0826a411340af5c7357a83c69e3f425beb23eaf232f8bb36dea3b7cad4a7ab8d735cee999c8ec5285005adf69dc85a6c34b042dd0308fe92b76dad5d6ac88c7b9259902c10f");
    nlohmann::json expectedJson = R"(
                {
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": [
                                      "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
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
                }
        )"_json;
    nlohmann::json parsedJson = nlohmann::json::parse(result.json());
    assertJSONEqual(expectedJson, parsedJson);
}

TEST(AptosSigner, CancelNftOfferTxSign) {
    // Successfully broadcasted https://explorer.aptoslabs.com/txn/0x0b8c64e6847c368e4c6bd2cce0e9eab378971b0ef2e3bc40cbd292910a80201d?network=testnet
    Proto::SigningInput input;
    input.set_sender("0x7968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30");
    input.set_sequence_number(21);
    auto& tf = *input.mutable_nft_message()->mutable_cancel_offer_nft();
    tf.set_receiver("0x783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee");
    tf.set_creator("0x9125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac");
    tf.set_collectionname("Topaz Troopers");
    tf.set_name("Topaz Trooper #20068");
    tf.set_property_version(0);
    input.set_max_gas_amount(3296766);
    input.set_gas_unit_price(100);
    input.set_expiration_timestamp_secs(3664390082);
    input.set_chain_id(2);
    auto privateKey = PrivateKey(parse_hex("5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(hex(result.raw_txn()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3015000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572731363616e63656c5f6f666665725f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada0000000002");
    ASSERT_EQ(hex(result.authenticator().signature()), "826722d374e276f618123e77da3ac024c89a3f97db9e09e19aa8ed06c3cdfc57d4a21c7890137f9a7c0447cc303447ba10ca5b1908e889071e0a68f48c0f260a");
    ASSERT_EQ(hex(result.encoded()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3015000000000000000200000000000000000000000000000000000000000000000000000000000000030f746f6b656e5f7472616e73666572731363616e63656c5f6f666665725f736372697074000520783135e8b00430253a22ba041d860c373d7a1501ccf7ac2d1ad37a8ed2775aee209125e4054d884fdc7296b66e12c0d63a7baa0d88c77e8e784987c0a967c670ac0f0e546f70617a2054726f6f706572731514546f70617a2054726f6f70657220233230303638080000000000000000fe4d3200000000006400000000000000c2276ada00000000020020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c40826722d374e276f618123e77da3ac024c89a3f97db9e09e19aa8ed06c3cdfc57d4a21c7890137f9a7c0447cc303447ba10ca5b1908e889071e0a68f48c0f260a");
    nlohmann::json expectedJson = R"(
                {
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
                    "sender": "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "21",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0x826722d374e276f618123e77da3ac024c89a3f97db9e09e19aa8ed06c3cdfc57d4a21c7890137f9a7c0447cc303447ba10ca5b1908e889071e0a68f48c0f260a",
                        "type": "ed25519_signature"
                    }
                }
        )"_json;
    nlohmann::json parsedJson = nlohmann::json::parse(result.json());
    assertJSONEqual(expectedJson, parsedJson);
}

TEST(AptosSigner, TxSign) {
    // Successfully broadcasted https://explorer.aptoslabs.com/txn/0xb4d62afd3862116e060dd6ad9848ccb50c2bc177799819f1d29c059ae2042467?network=devnet
    Proto::SigningInput input;
    input.set_sender("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30");
    input.set_sequence_number(99);
    auto& tf = *input.mutable_transfer();
    tf.set_to("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30");
    tf.set_amount(1000);
    input.set_max_gas_amount(3296766);
    input.set_gas_unit_price(100);
    input.set_expiration_timestamp_secs(3664390082);
    input.set_chain_id(33);
    auto privateKey = PrivateKey(parse_hex("5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(hex(result.raw_txn()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000021");
    ASSERT_EQ(hex(result.authenticator().signature()), "5707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01");
    ASSERT_EQ(hex(result.encoded()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c405707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01");
    nlohmann::json expectedJson = R"(
                {
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": ["0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30","1000"],
                        "function": "0x1::aptos_account::transfer",
                        "type": "entry_function_payload",
                        "type_arguments": []
                    },
                    "sender": "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "99",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0x5707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01",
                        "type": "ed25519_signature"
                    }
                }
        )"_json;
    nlohmann::json parsedJson = nlohmann::json::parse(result.json());
    assertJSONEqual(expectedJson, parsedJson);
}

TEST(AptosSigner, CreateAccount) {
    // Successfully broadcasted: https://explorer.aptoslabs.com/txn/0x477141736de6b0936a6c3734e4d6fd018c7d21f1f28f99028ef0bc6881168602?network=Devnet
    Proto::SigningInput input;
    input.set_sender("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30");
    input.set_sequence_number(0);
    auto& tf = *input.mutable_create_account();
    tf.set_auth_key("0x3aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30e");
    input.set_max_gas_amount(3296766);
    input.set_gas_unit_price(100);
    input.set_expiration_timestamp_secs(3664390082);
    input.set_chain_id(33);
    auto privateKey = PrivateKey(parse_hex("5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(hex(result.raw_txn()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3000000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e740e6372656174655f6163636f756e740001203aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30efe4d3200000000006400000000000000c2276ada0000000021");
    ASSERT_EQ(hex(result.authenticator().signature()), "fcba3dfbec76721454ef414955f09f159660a13886b4edd8c579e3c779c29073afe7b25efa3fef9b21c2efb1cf16b4247fc0e5c8f63fdcd1c8d87f5d59f44501");
    ASSERT_EQ(hex(result.encoded()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3000000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e740e6372656174655f6163636f756e740001203aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30efe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c40fcba3dfbec76721454ef414955f09f159660a13886b4edd8c579e3c779c29073afe7b25efa3fef9b21c2efb1cf16b4247fc0e5c8f63fdcd1c8d87f5d59f44501");
    nlohmann::json expectedJson = R"(
                {
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": ["0x3aa1672641a4e17b3d913b4c0301e805755a80b12756fc729c5878f12344d30e"],
                        "function": "0x1::aptos_account::create_account",
                        "type": "entry_function_payload",
                        "type_arguments": []
                    },
                    "sender": "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "0",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0xfcba3dfbec76721454ef414955f09f159660a13886b4edd8c579e3c779c29073afe7b25efa3fef9b21c2efb1cf16b4247fc0e5c8f63fdcd1c8d87f5d59f44501",
                        "type": "ed25519_signature"
                    }
                }
        )"_json;
    nlohmann::json parsedJson = nlohmann::json::parse(result.json());
    assertJSONEqual(expectedJson, parsedJson);
}

TEST(AptosSigner, BlindSign) {
    // successfully broadcasted https://explorer.aptoslabs.com/txn/0xd95857a9e644528708778a3a0a6e13986751944fca30eaac98853c1655de0422?network=Devnet
    // encoded submission with:
    // curl --location --request POST 'https://fullnode.devnet.aptoslabs.com/v1/transactions/encode_submission' \
    //--header 'Content-Type: application/json' \
    //--header 'Cookie: AWSALB=0zI2zWypvEr0I3sGM6vnyHSxYO1D0aaMXfyA/2VwhA291aJJ80Yz67Fur50sXPFBI8dKKID4p8DShj1KkEXPY/NGAylpOj1EG2M2Qjuu1B38Q5C+dZW2CHT+IAZ5; AWSALBCORS=0zI2zWypvEr0I3sGM6vnyHSxYO1D0aaMXfyA/2VwhA291aJJ80Yz67Fur50sXPFBI8dKKID4p8DShj1KkEXPY/NGAylpOj1EG2M2Qjuu1B38Q5C+dZW2CHT+IAZ5' \
    //--data-raw '{
    //    "expiration_timestamp_secs": "3664390082",
    //    "gas_unit_price": "100",
    //    "max_gas_amount": "3296766",
    //    "payload": {
    //        "function": "0x4633134869a61c41ad42eaca028d71c5b8b4109ffd69e1aa99c35a621b298837::pool::swap_y_to_x",
    //        "type_arguments": [
    //            "0xdeae46f81671e76f444e2ce5a299d9e1ea06a8fa26e81dfd49aa7fa5a5a60e01::devnet_coins::DevnetUSDT",
    //            "0x1::aptos_coin::AptosCoin"
    //        ],
    //        "arguments": [
    //            "100000000",
    //            "0"
    //        ],
    //        "type": "entry_function_payload"
    //    },
    //    "sender": "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
    //    "sequence_number": "2"
    //}'
    Proto::SigningInput input;
    input.set_any_encoded("0xb5e97db07fa0bd0e5598aa3643a9bc6f6693bddc1a9fec9e674a461eaa00b19307968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f300200000000000000024633134869a61c41ad42eaca028d71c5b8b4109ffd69e1aa99c35a621b29883704706f6f6c0b737761705f795f746f5f780207deae46f81671e76f444e2ce5a299d9e1ea06a8fa26e81dfd49aa7fa5a5a60e010c6465766e65745f636f696e730a4465766e657455534454000700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e00020800e1f50500000000080000000000000000fe4d3200000000006400000000000000c2276ada0000000021");
    auto privateKey = PrivateKey(parse_hex("5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(hex(result.raw_txn()), "b5e97db07fa0bd0e5598aa3643a9bc6f6693bddc1a9fec9e674a461eaa00b19307968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f300200000000000000024633134869a61c41ad42eaca028d71c5b8b4109ffd69e1aa99c35a621b29883704706f6f6c0b737761705f795f746f5f780207deae46f81671e76f444e2ce5a299d9e1ea06a8fa26e81dfd49aa7fa5a5a60e010c6465766e65745f636f696e730a4465766e657455534454000700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e00020800e1f50500000000080000000000000000fe4d3200000000006400000000000000c2276ada0000000021");
    ASSERT_EQ(hex(result.authenticator().signature()), "9e81026fdd43986f4d5588afdab875cd18b64dc15b3489fcc00ed46fc361915b27e23e0cefe6d23698ee76a562915fe85e99185dbc1dd29ba720f7fad144af0b");
    ASSERT_EQ(hex(result.encoded()), "b5e97db07fa0bd0e5598aa3643a9bc6f6693bddc1a9fec9e674a461eaa00b19307968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f300200000000000000024633134869a61c41ad42eaca028d71c5b8b4109ffd69e1aa99c35a621b29883704706f6f6c0b737761705f795f746f5f780207deae46f81671e76f444e2ce5a299d9e1ea06a8fa26e81dfd49aa7fa5a5a60e010c6465766e65745f636f696e730a4465766e657455534454000700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e00020800e1f50500000000080000000000000000fe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c409e81026fdd43986f4d5588afdab875cd18b64dc15b3489fcc00ed46fc361915b27e23e0cefe6d23698ee76a562915fe85e99185dbc1dd29ba720f7fad144af0b");
    nlohmann::json expectedJson = R"(
                {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0x9e81026fdd43986f4d5588afdab875cd18b64dc15b3489fcc00ed46fc361915b27e23e0cefe6d23698ee76a562915fe85e99185dbc1dd29ba720f7fad144af0b",
                        "type": "ed25519_signature"
                }
        )"_json;
    nlohmann::json parsedJson = nlohmann::json::parse(result.json());
    assertJSONEqual(expectedJson, parsedJson);
}

TEST(AptosSigner, TokenTxSign) {
    // Successfully broadcasted https://explorer.aptoslabs.com/txn/0xb5b383a5c7f99b2edb3bed9533f8169a89051b149d65876a82f4c0b9bf78a15b?network=Devnet
    Proto::SigningInput input;
    input.set_sender("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30");
    input.set_sequence_number(24);
    auto& tf = *input.mutable_token_transfer();
    tf.set_to("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30");
    tf.set_amount(100000);
    tf.mutable_function()->set_account_address("0x43417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b9");
    tf.mutable_function()->set_module("coins");
    tf.mutable_function()->set_name("BTC");
    input.set_max_gas_amount(3296766);
    input.set_gas_unit_price(100);
    input.set_expiration_timestamp_secs(3664390082);
    input.set_chain_id(32);
    auto privateKey = PrivateKey(parse_hex("5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(hex(result.raw_txn()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30180000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010743417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b905636f696e730342544300022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008a086010000000000fe4d3200000000006400000000000000c2276ada0000000020");
    ASSERT_EQ(hex(result.authenticator().signature()), "7643ec8aae6198bd13ca6ea2962265859cba5a228e7d181131f6c022700dd02a7a04dc0345ad99a0289e5ab80b130b3864e6404079980bc226f1a13aee7d280a");
    ASSERT_EQ(hex(result.encoded()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30180000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010743417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b905636f696e730342544300022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008a086010000000000fe4d3200000000006400000000000000c2276ada00000000200020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c407643ec8aae6198bd13ca6ea2962265859cba5a228e7d181131f6c022700dd02a7a04dc0345ad99a0289e5ab80b130b3864e6404079980bc226f1a13aee7d280a");
    nlohmann::json expectedJson = R"(
                {
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": ["0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30","100000"],
                        "function": "0x1::coin::transfer",
                        "type": "entry_function_payload",
                        "type_arguments": ["0x43417434fd869edee76cca2a4d2301e528a1551b1d719b75c350c3c97d15b8b9::coins::BTC"]
                    },
                    "sender": "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "24",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0x7643ec8aae6198bd13ca6ea2962265859cba5a228e7d181131f6c022700dd02a7a04dc0345ad99a0289e5ab80b130b3864e6404079980bc226f1a13aee7d280a",
                        "type": "ed25519_signature"
                    }
                }
        )"_json;
    nlohmann::json parsedJson = nlohmann::json::parse(result.json());
    assertJSONEqual(expectedJson, parsedJson);
}

} // namespace TW::Aptos::tests
