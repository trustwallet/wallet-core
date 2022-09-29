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

} // namespace TW::Aptos::tests
