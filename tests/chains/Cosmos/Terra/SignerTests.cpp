// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Base64.h"
#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"
#include "Cosmos/ProtobufSerialization.h"
#include "uint256.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

namespace TW::Cosmos::tests {

TEST(TerraClassicSigner, SignSendTx) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::JSON);
    input.set_account_number(1037);
    input.set_chain_id("columbus-5");
    input.set_memo("");
    input.set_sequence(2);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2", fromAddress));
    Address toAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", toAddress));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("luna");
    amountOfTx->set_amount("1000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("luna");
    amountOfFee->set_amount("200");

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);
    EXPECT_EQ(json, R"({"accountNumber":"1037","chainId":"columbus-5","fee":{"amounts":[{"denom":"luna","amount":"200"}],"gas":"200000"},"sequence":"2","messages":[{"sendCoinsMessage":{"fromAddress":"terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2","toAddress":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf","amounts":[{"denom":"luna","amount":"1000000"}]}}]})");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeTerra);

    assertJSONEqual(output.json(), R"(
        {
            "mode": "block",
            "tx": {
                "fee": {
                    "amount": [
                        {
                            "amount": "200",
                            "denom": "luna"
                        }
                    ],
                    "gas": "200000"
                },
                "memo": "",
                "msg": [
                    {
                        "type": "cosmos-sdk/MsgSend",
                        "value": {
                            "amount": [
                                {
                                    "amount": "1000000",
                                    "denom": "luna"
                                }
                            ],
                            "from_address": "terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2",
                            "to_address": "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"
                        }
                    }
                ],
                "signatures": [
                    {
                        "pub_key": {
                            "type": "tendermint/PubKeySecp256k1",
                            "value": "AlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3F"
                        },
                        "signature": "ofdIsLJzkODcQwLG89eE2g4HOaUmfKPh/08t07ehKPUqRMl4rVonzo73mkOvqtrHWjdtB+6t6R8DGudPpb6bRg=="
                    }
                ]
            }
        }
    )");
    EXPECT_EQ(hex(output.signature()), "a1f748b0b27390e0dc4302c6f3d784da0e0739a5267ca3e1ff4f2dd3b7a128f52a44c978ad5a27ce8ef79a43afaadac75a376d07eeade91f031ae74fa5be9b46");
    EXPECT_EQ(output.serialized(), "");
    EXPECT_EQ(output.error_message(), "");
}

TEST(TerraClassicSigner, SignWasmTransferTxProtobuf_9FF3F0) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(3407705);
    input.set_chain_id("columbus-5");
    input.set_memo("");
    input.set_sequence(3);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", fromAddress));
    Address toAddress;
    ASSERT_TRUE(Address::decode("terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp", toAddress));
    const auto tokenContractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76"; // ANC

    auto msg = input.add_messages();
    auto& message = *msg->mutable_wasm_terra_execute_contract_transfer_message();
    message.set_sender_address(fromAddress.string());
    message.set_contract_address(tokenContractAddress);
    const auto amount = store(uint256_t(250000), 0);
    message.set_amount(amount.data(), amount.size());
    message.set_recipient_address(toAddress.string());

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uluna");
    amountOfFee->set_amount("3000");

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);
    assertJSONEqual(json, R"(
        {
            "signingMode": "Protobuf",
            "accountNumber": "3407705",
            "chainId": "columbus-5",
            "fee": {
                "amounts": [
                    {
                        "denom": "uluna",
                        "amount": "3000"
                    }
                ],
                "gas": "200000"
            },
            "sequence": "3",
            "messages": [
                {
                    "wasmTerraExecuteContractTransferMessage": {
                        "senderAddress": "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf",
                        "contractAddress": "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76",
                        "amount": "A9CQ",
                        "recipientAddress": "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp"
                    }
                }
            ]
        }
    )");

    auto privateKey = parse_hex("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeTerra);

    // https://finder.terra.money/mainnet/tx/9FF3F0A16879254C22EB90D8B4D6195467FE5014381FD36BD3C23CA6698FE94B
    // curl -H 'Content-Type: application/json' --data-binary '{"mode": "BROADCAST_MODE_BLOCK","tx_bytes": "CogCCo..wld8"})' https://<lcd-node-url>/cosmos/tx/v1beta1/txs
    assertJSONEqual(output.serialized(), R"(
        {
            "tx_bytes": "CucBCuQBCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBK5AQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2Glt7InRyYW5zZmVyIjp7ImFtb3VudCI6IjI1MDAwMCIsInJlY2lwaWVudCI6InRlcnJhMWpsZ2FxeTludm4yaGY1dDJzcmE5eWN6OHM3N3duZjlsMGttZ2NwIn19EmcKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQNwZjrHsPmJKW/rXOWfukpQ1+lOHOJW3/IlFFnKLNmsABIECgIIARgDEhMKDQoFdWx1bmESBDMwMDAQwJoMGkAaprIEMLPH2HmFdwFGoaipb2GIyhXt6ombz+WMnG2mORBI6gFt0M+IymYgzZz6w1SW52R922yafDnn7yXfutRw",
            "mode": "BROADCAST_MODE_BLOCK"
        }
    )");
    EXPECT_EQ(hex(output.signature()), "1aa6b20430b3c7d87985770146a1a8a96f6188ca15edea899bcfe58c9c6da6391048ea016dd0cf88ca6620cd9cfac35496e7647ddb6c9a7c39e7ef25dfbad470");
    EXPECT_EQ(output.json(), "");
}

TEST(TerraClassicSigner, SignWasmTransferTxJson_078E90) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::JSON);
    input.set_account_number(3407705);
    input.set_chain_id("columbus-5");
    input.set_memo("");
    input.set_sequence(2);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", fromAddress));
    Address toAddress;
    ASSERT_TRUE(Address::decode("terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp", toAddress));
    const auto tokenContractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76"; // ANC

    auto msg = input.add_messages();
    auto& message = *msg->mutable_wasm_terra_execute_contract_transfer_message();
    message.set_sender_address(fromAddress.string());
    message.set_contract_address(tokenContractAddress);
    const auto amount = store(250000);
    message.set_amount(amount.data(), amount.size());
    message.set_recipient_address(toAddress.string());

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uluna");
    amountOfFee->set_amount("3000");

    auto privateKey = parse_hex("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeTerra);

    // https://finder.terra.money/mainnet/tx/078E90458061611F6FD8B708882B55FF5C1FFB3FCE61322107A0A0DE39FC0F3E
    // curl -H 'Content-Type: application/json' --data-binary '{"mode": "block","tx":{...}}' https://<lcd-node-url>/txs
    assertJSONEqual(output.json(), R"(
        {
            "mode": "block",
            "tx":
                {
                    "fee": {"amount":[{"amount": "3000","denom": "uluna"}],"gas": "200000"},
                    "memo": "",
                    "msg":
                        [
                            {
                                "type": "wasm/MsgExecuteContract",
                                "value":
                                    {
                                        "sender": "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf",
                                        "contract": "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76",
                                        "execute_msg":
                                            {
                                                "transfer":
                                                    {
                                                        "amount": "250000",
                                                        "recipient": "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp"
                                                    }
                                            },
                                        "coins": []
                                    }
                            }
                        ],
                    "signatures":
                        [
                            {
                                "pub_key":
                                    {
                                        "type": "tendermint/PubKeySecp256k1",
                                        "value": "A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"
                                    },
                                "signature": "BjETdtbA97Wv1zvcsCV1tM+bdYKC8O3uGTk4mMRv6pBJB2y/Ds7qoS7s/zrkhYak1YChklQetHsI30XRXzGIkg=="
                            }
                        ]
                }
        })");
    EXPECT_EQ(hex(output.signature()), "06311376d6c0f7b5afd73bdcb02575b4cf9b758282f0edee19393898c46fea9049076cbf0eceeaa12eecff3ae48586a4d580a192541eb47b08df45d15f318892");
    EXPECT_EQ(output.serialized(), "");
}

TEST(TerraClassicSigner, SignWasmGeneric_EC4F85) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(3407705);
    input.set_chain_id("columbus-5");
    input.set_memo("");
    input.set_sequence(7);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", fromAddress));
    Address toAddress;
    ASSERT_TRUE(Address::decode("terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp", toAddress));
    const auto tokenContractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76"; // ANC
    const auto txMessage = R"({"transfer": { "amount": "250000", "recipient": "terra1d7048csap4wzcv5zm7z6tdqem2agyp9647vdyj" } })";

    auto msg = input.add_messages();
    auto& message = *msg->mutable_wasm_terra_execute_contract_generic();
    message.set_sender_address(fromAddress.string());
    message.set_contract_address(tokenContractAddress);
    message.set_execute_msg(txMessage);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uluna");
    amountOfFee->set_amount("3000");

    auto privateKey = parse_hex("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeTerra);

    // https://finder.terra.money/mainnet/tx/EC4F8532847E4D6AF016E6F6D3F027AE7FB6FF0B533C5132B01382D83B214A6F
    // curl -H 'Content-Type: application/json' --data-binary '{"mode": "BROADCAST_MODE_BLOCK","tx_bytes": "Cu4BC...iVt"})' https://<lcd-node-url>/cosmos/tx/v1beta1/txs
    assertJSONEqual(output.serialized(), R"(
        {
            "tx_bytes": "Cu4BCusBCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBLAAQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2GmJ7InRyYW5zZmVyIjogeyAiYW1vdW50IjogIjI1MDAwMCIsICJyZWNpcGllbnQiOiAidGVycmExZDcwNDhjc2FwNHd6Y3Y1em03ejZ0ZHFlbTJhZ3lwOTY0N3ZkeWoiIH0gfRJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDcGY6x7D5iSlv61zln7pKUNfpThziVt/yJRRZyizZrAASBAoCCAEYBxITCg0KBXVsdW5hEgQzMDAwEMCaDBpAkPsS7xlSng2LMc9KiD1soN5NLaDcUh8I9okPmsdJN3le1B7yxRGNB4aQfhaRl/8Z0r5vitRT0AWuxDasd8wcFw==",
            "mode": "BROADCAST_MODE_BLOCK"
        }
    )");

    EXPECT_EQ(hex(output.signature()), "90fb12ef19529e0d8b31cf4a883d6ca0de4d2da0dc521f08f6890f9ac74937795ed41ef2c5118d0786907e169197ff19d2be6f8ad453d005aec436ac77cc1c17");
    EXPECT_EQ(output.json(), "");
}

TEST(TerraClassicSigner, SignWasmGenericWithCoins_6651FC) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(3407705);
    input.set_chain_id("columbus-5");
    input.set_memo("");
    input.set_sequence(9);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", fromAddress));
    Address toAddress;
    ASSERT_TRUE(Address::decode("terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp", toAddress));
    const auto tokenContractAddress = "terra1sepfj7s0aeg5967uxnfk4thzlerrsktkpelm5s"; // ANC Market
    const auto txMessage = R"({ "deposit_stable": {} })";

    auto msg = input.add_messages();
    auto& message = *msg->mutable_wasm_terra_execute_contract_generic();
    message.set_sender_address(fromAddress.string());
    message.set_contract_address(tokenContractAddress);
    message.set_execute_msg(txMessage);

    auto amount = message.add_coins();
    amount->set_denom("uusd");
    amount->set_amount("1000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(600000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uluna");
    amountOfFee->set_amount("7000");

    auto privateKey = parse_hex("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeTerra);

    // https://finder.terra.money/mainnet/tx/6651FCE0EE5C6D6ACB655CC49A6FD5E939FB082862854616EA0642475BCDD0C9
    // curl -H 'Content-Type: application/json' --data-binary '{"mode": "BROADCAST_MODE_BLOCK","tx_bytes": "CrIBCq8B.....0NWg=="})' https://<lcd-node-url>/cosmos/tx/v1beta1/txs
    assertJSONEqual(output.serialized(), R"(
        {
            "tx_bytes": "CrIBCq8BCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBKEAQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMXNlcGZqN3MwYWVnNTk2N3V4bmZrNHRoemxlcnJza3RrcGVsbTVzGhh7ICJkZXBvc2l0X3N0YWJsZSI6IHt9IH0qDAoEdXVzZBIEMTAwMBJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDcGY6x7D5iSlv61zln7pKUNfpThziVt/yJRRZyizZrAASBAoCCAEYCRITCg0KBXVsdW5hEgQ3MDAwEMDPJBpAGyi7f1ioY8XV6pjFq1s86Om4++CIUnd3rLHif2iopCcYvX0mLkTlQ6NUERg8nWTYgXcj6fOTO/ptgPuAtv0NWg==",
            "mode": "BROADCAST_MODE_BLOCK"
        }
    )");

    EXPECT_EQ(hex(output.signature()), "1b28bb7f58a863c5d5ea98c5ab5b3ce8e9b8fbe088527777acb1e27f68a8a42718bd7d262e44e543a35411183c9d64d8817723e9f3933bfa6d80fb80b6fd0d5a");
    EXPECT_EQ(output.json(), "");
}

TEST(TerraClassicSigner, SignWasmSendTxProtobuf) {
    auto input = Proto::SigningInput();
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(3407705);
    input.set_chain_id("columbus-5");
    input.set_memo("");
    input.set_sequence(4);

    Address fromAddress;
    ASSERT_TRUE(Address::decode("terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf", fromAddress));
    Address toAddress;
    ASSERT_TRUE(Address::decode("terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp", toAddress));
    const auto tokenContractAddress = "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76"; // ANC

    auto msg = input.add_messages();
    auto& message = *msg->mutable_wasm_terra_execute_contract_send_message();
    message.set_sender_address(fromAddress.string());
    message.set_contract_address(tokenContractAddress);
    const auto amount = store(uint256_t(250000), 0);
    message.set_amount(amount.data(), amount.size());
    message.set_recipient_contract_address(toAddress.string());
    const auto msgMsg = Base64::encode(data(std::string(R"({"some_message":{}})")));
    EXPECT_EQ(msgMsg, "eyJzb21lX21lc3NhZ2UiOnt9fQ==");
    message.set_msg(msgMsg);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uluna");
    amountOfFee->set_amount("3000");

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);
    assertJSONEqual(json, R"(
        {
            "signingMode": "Protobuf",
            "accountNumber": "3407705",
            "chainId": "columbus-5",
            "fee": {
                "amounts": [
                    {
                        "denom": "uluna",
                        "amount": "3000"
                    }
                ],
                "gas": "200000"
            },
            "sequence": "4",
            "messages": [
                {
                    "wasmTerraExecuteContractSendMessage": {
                        "senderAddress": "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf",
                        "contractAddress": "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76",
                        "amount": "A9CQ",
                        "recipientContractAddress": "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp",
                        "msg": "eyJzb21lX21lc3NhZ2UiOnt9fQ=="
                    }
                }
            ]
        }
    )");

    auto privateKey = parse_hex("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input, TWCoinTypeTerra);

    // https://finder.terra.money/mainnet/tx/9FF3F0A16879254C22EB90D8B4D6195467FE5014381FD36BD3C23CA6698FE94B
    // curl -H 'Content-Type: application/json' --data-binary '{"mode": "BROADCAST_MODE_BLOCK","tx_bytes": "CogCCo..wld8"})' https://<lcd-node-url>/cosmos/tx/v1beta1/txs
    assertJSONEqual(output.serialized(), R"(
        {
            "tx_bytes": "CocCCoQCCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBLZAQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2Gnt7InNlbmQiOnsiYW1vdW50IjoiMjUwMDAwIiwiY29udHJhY3QiOiJ0ZXJyYTFqbGdhcXk5bnZuMmhmNXQyc3JhOXljejhzNzd3bmY5bDBrbWdjcCIsIm1zZyI6ImV5SnpiMjFsWDIxbGMzTmhaMlVpT250OWZRPT0ifX0SZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awAEgQKAggBGAQSEwoNCgV1bHVuYRIEMzAwMBDAmgwaQL6NByKeRZsyq5g6CTMdmPqiM77nOe9uLO8FjpetFgkBFiG3Le7ieZZ+4vCMhD1bcFgMwSHibFI/uPil847U/+g=",
            "mode": "BROADCAST_MODE_BLOCK"
        }
    )");
    EXPECT_EQ(hex(output.signature()), "be8d07229e459b32ab983a09331d98faa233bee739ef6e2cef058e97ad1609011621b72deee279967ee2f08c843d5b70580cc121e26c523fb8f8a5f38ed4ffe8");
    EXPECT_EQ(output.json(), "");
}

TEST(TerraClassicSigner, SignWasmTerraTransferPayload) {
    auto proto = Proto::Message_WasmTerraExecuteContractTransfer();
    proto.set_recipient_address("recipient=address");
    const auto amount = store(uint256_t(250000), 0);
    proto.set_amount(amount.data(), amount.size());

    const auto payload = Protobuf::wasmTerraExecuteTransferPayload(proto);

    assertJSONEqual(payload.dump(), R"(
        {
            "transfer":
                {
                    "amount": "250000",
                    "recipient": "recipient=address"
                }
        }
    )");
}

} // namespace TW::Cosmos::tests
