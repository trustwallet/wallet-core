// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HexCoding.h"
#include "Base64.h"
#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "Cosmos/Signer.h"
#include "Cosmos/ProtobufSerialization.h"
#include "uint256.h"
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Cosmos;


TEST(TerraSigner, SignSendTx) {
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
    amountOfTx->set_amount(1000000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("luna");
    amountOfFee->set_amount(200);

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);
    EXPECT_EQ(json, R"({"accountNumber":"1037","chainId":"columbus-5","fee":{"amounts":[{"denom":"luna","amount":"200"}],"gas":"200000"},"sequence":"2","messages":[{"sendCoinsMessage":{"fromAddress":"terra1hsk6jryyqjfhp5dhc55tc9jtckygx0ep37hdd2","toAddress":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf","amounts":[{"denom":"luna","amount":"1000000"}]}}]})");

    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

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
    EXPECT_EQ(output.error(), "");
}

TEST(TerraSigner, SignWasmTransferTxProtobuf_9FF3F0) {
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
    amountOfFee->set_amount(3000);

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

    auto output = Signer::sign(input);

    // https://finder.terra.money/mainnet/tx/9FF3F0A16879254C22EB90D8B4D6195467FE5014381FD36BD3C23CA6698FE94B
    // curl -H 'Content-Type: application/json' --data-binary '{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CogCCo..wld8"})' https://<lcd-node-url>/cosmos/tx/v1beta1/txs
    assertJSONEqual(output.serialized(), R"(
        {
            "tx_bytes":"CucBCuQBCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBK5AQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2Glt7InRyYW5zZmVyIjp7ImFtb3VudCI6IjI1MDAwMCIsInJlY2lwaWVudCI6InRlcnJhMWpsZ2FxeTludm4yaGY1dDJzcmE5eWN6OHM3N3duZjlsMGttZ2NwIn19EmcKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQNwZjrHsPmJKW/rXOWfukpQ1+lOHOJW3/IlFFnKLNmsABIECgIIARgDEhMKDQoFdWx1bmESBDMwMDAQwJoMGkAaprIEMLPH2HmFdwFGoaipb2GIyhXt6ombz+WMnG2mORBI6gFt0M+IymYgzZz6w1SW52R922yafDnn7yXfutRw",
            "mode":"BROADCAST_MODE_BLOCK"
        }
    )");
    EXPECT_EQ(hex(output.signature()), "1aa6b20430b3c7d87985770146a1a8a96f6188ca15edea899bcfe58c9c6da6391048ea016dd0cf88ca6620cd9cfac35496e7647ddb6c9a7c39e7ef25dfbad470");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error(), "");
}

TEST(TerraSigner, SignWasmTransferTxJson_078E90) {
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
    amountOfFee->set_amount(3000);

    auto privateKey = parse_hex("cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Signer::sign(input);

    // https://finder.terra.money/mainnet/tx/078E90458061611F6FD8B708882B55FF5C1FFB3FCE61322107A0A0DE39FC0F3E
    // curl -H 'Content-Type: application/json' --data-binary '{"mode":"block","tx":{...}}' https://<lcd-node-url>/txs
    assertJSONEqual(output.json(), R"(
        {
            "mode":"block",
            "tx":
                {
                    "fee": {"amount":[{"amount":"3000","denom":"uluna"}],"gas":"200000"},
                    "memo":"",
                    "msg":
                        [
                            {
                                "type":"wasm/MsgExecuteContract",
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
    EXPECT_EQ(output.error(), "");
}

TEST(TerraSigner, SignWasmTerraTransferPayload) {
    auto proto = Proto::Message_WasmTerraExecuteContractTransfer();
    proto.set_recipient_address("recipient=address");
    const auto amount = store(uint256_t(250000), 0);
    proto.set_amount(amount.data(), amount.size());

    const auto payload = wasmTerraExecuteTransferPayload(proto);

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
