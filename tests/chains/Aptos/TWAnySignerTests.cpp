// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Aptos.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <nlohmann/json.hpp>
#include "TestUtilities.h"

#include <gtest/gtest.h>

namespace TW::Aptos::tests {

TEST(TWAnySignerAptos, TxSign) {
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
    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAptos);
    ASSERT_EQ(hex(output.raw_txn()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000021");
    ASSERT_EQ(hex(output.authenticator().signature()), "5707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01");
    ASSERT_EQ(hex(output.encoded()), "07968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada00000000210020ea526ba1710343d953461ff68641f1b7df5f23b9042ffa2d2a798d3adb3f3d6c405707246db31e2335edc4316a7a656a11691d1d1647f6e864d1ab12f43428aaaf806cf02120d0b608cdd89c5c904af7b137432aacdd60cc53f9fad7bd33578e01");
    nlohmann::json expectedJson = R"(
                {
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
                }
        )"_json;
    nlohmann::json parsedJson = nlohmann::json::parse(output.json());
    assertJSONEqual(expectedJson, parsedJson);
}

} // namespace TW::Aptos::tests
