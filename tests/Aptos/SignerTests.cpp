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
#include <nlohmann/json.hpp>

#include <gtest/gtest.h>

namespace TW::Aptos::tests {

TEST(AptosSigner, DummyTxSign) {
    Proto::SigningInput input;
    input.set_sender("0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b");
    input.set_sequence_number(1);
    auto& tf = *input.mutable_transfer();
    tf.set_to("0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b");
    tf.set_amount(1000);
    input.set_max_gas_amount(1);
    input.set_gas_unit_price(1);
    input.set_expiration_timestamp_secs(1);
    input.set_chain_id(1);
    auto privateKey = PrivateKey(parse_hex("7f2634c0e2414a621e96e39c41d09021700cee12ee43328ed094c5580cd0bd6f"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(hex(result.raw_txn()), "eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b010000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e000220eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b08e80300000000000001000000000000000100000000000000010000000000000001");
    ASSERT_EQ(hex(result.authenticator().signature()), "9d3bd902bd358364c43fa65ece335dd4411527e72e1c6deb9148744eaa24e39b6bd74ff6b0195114243bdd2ee3a98511ff05883d9e79161b2b8f5029d883c309");
    ASSERT_EQ(hex(result.encoded()), "eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b010000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e000220eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b08e803000000000000010000000000000001000000000000000100000000000000010020633e5c7e355bdd484706436ce1f06fdf280bd7c2229a7f9b6489684412c6967c409d3bd902bd358364c43fa65ece335dd4411527e72e1c6deb9148744eaa24e39b6bd74ff6b0195114243bdd2ee3a98511ff05883d9e79161b2b8f5029d883c309");
    nlohmann::json expectedJson = R"(
        {
            "sender": "0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b",
            "sequence_number": "1",
            "max_gas_amount": "1",
            "gas_unit_price": "1",
            "expiration_timestamp_secs": "1",
            "payload": {
                "type":"entry_function_payload",
                "function": "0x1::coin::transfer",
                "type_arguments":["0x1::aptos_coin::AptosCoin"],
                "arguments": ["0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b", "1000"]
            },
            "signature": {
                "type": "ed25519_signature",
                "public_key": "0x633e5c7e355bdd484706436ce1f06fdf280bd7c2229a7f9b6489684412c6967c",
                "signature": "0x9d3bd902bd358364c43fa65ece335dd4411527e72e1c6deb9148744eaa24e39b6bd74ff6b0195114243bdd2ee3a98511ff05883d9e79161b2b8f5029d883c309"
            }
        }
        )"_json;
    nlohmann::json parsedJson = nlohmann::json::parse(result.json());
    ASSERT_EQ(expectedJson, parsedJson);
}

TEST(AptosSigner, TxSign) {
    // Successfully broadcasted https://explorer.aptoslabs.com/txn/0xbb3b3c33781c27e486afa2db854fb0a5c846d0967672feb2c6c3297a2b14e1ce?network=Devnet
    Proto::SigningInput input;
    input.set_sender("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30");
    input.set_sequence_number(15);
    auto& tf = *input.mutable_transfer();
    tf.set_to("0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30");
    tf.set_amount(1000);
    input.set_max_gas_amount(3296766);
    input.set_gas_unit_price(100);
    input.set_expiration_timestamp_secs(3664390082);
    input.set_chain_id(32);
    auto privateKey = PrivateKey(parse_hex("5d996aa76b3212142792d9130796cd2e11e3c445a93118c08414df4f66bc60ec"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(hex(result.raw_txn()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f300f0000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e00022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000020");
    ASSERT_EQ(hex(result.authenticator().signature()), "2ac7acac0e597d04017b8d9ecad1ee7c2e07f3346957e507ac06508fe5c42c74892a347875d8d8826485a6e9b267bb7a0f24212be29c333c941c5db79c93ce05");
    ASSERT_EQ(hex(result.encoded()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f300f0000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e00022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada00000000200020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c402ac7acac0e597d04017b8d9ecad1ee7c2e07f3346957e507ac06508fe5c42c74892a347875d8d8826485a6e9b267bb7a0f24212be29c333c941c5db79c93ce05");
    nlohmann::json expectedJson = R"(
                {
                    "expiration_timestamp_secs": "3664390082",
                    "gas_unit_price": "100",
                    "max_gas_amount": "3296766",
                    "payload": {
                        "arguments": ["0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30","1000"],
                        "function": "0x1::coin::transfer",
                        "type": "entry_function_payload",
                        "type_arguments": ["0x1::aptos_coin::AptosCoin"]
                    },
                    "sender": "0x07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f30",
                    "sequence_number": "15",
                    "signature": {
                        "public_key": "0xea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c",
                        "signature": "0x2ac7acac0e597d04017b8d9ecad1ee7c2e07f3346957e507ac06508fe5c42c74892a347875d8d8826485a6e9b267bb7a0f24212be29c333c941c5db79c93ce05",
                        "type": "ed25519_signature"
                    }
                }
        )"_json;
    nlohmann::json parsedJson = nlohmann::json::parse(result.json());
    ASSERT_EQ(expectedJson, parsedJson);
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
    ASSERT_EQ(expectedJson, parsedJson);
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
    ASSERT_EQ(expectedJson, parsedJson);
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
    ASSERT_EQ(expectedJson, parsedJson);
}

} // namespace TW::Aptos::tests
