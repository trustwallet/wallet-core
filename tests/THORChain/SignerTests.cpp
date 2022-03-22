// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "proto/Cosmos.pb.h"
#include "THORChain/Signer.h"
#include "HexCoding.h"
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

using namespace TW;

TEST(THORChainSigner, SignTx) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_memo("memo1234");
    input.set_sequence(123);

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address("thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r");
    message.set_to_address("thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn");
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("rune");
    amountOfTx->set_amount(50000000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(2000000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("rune");
    amountOfFee->set_amount(200);

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    assertJSONEqual(json, R"(
        {
            "fee": {
                "amounts": [
                    {
                        "denom": "rune",
                        "amount": "200"
                    }
                ],
                "gas": "2000000"
            },
            "memo": "memo1234",
            "messages": [
                {
                    "sendCoinsMessage": {
                        "fromAddress": "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
                        "toAddress": "thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn",
                        "amounts": [
                            {
                                "denom": "rune",
                                "amount": "50000000"
                            }
                        ]
                    }
                }
            ],
            "sequence": "123"
        }
    )");

    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = THORChain::Signer::sign(input);

    assertJSONEqual(output.json(), R"(
        {
            "mode": "block",
            "tx": {
                "fee": {
                    "amount": [
                        {
                            "amount": "200",
                            "denom": "rune"
                        }
                    ],
                    "gas": "2000000"
                },
                "memo": "memo1234",
                "msg": [
                    {
                        "type": "thorchain/MsgSend",
                        "value": {
                            "amount": [
                                {
                                    "amount": "50000000",
                                    "denom": "rune"
                                }
                            ],
                            "from_address": "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
                            "to_address": "thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"
                        }
                    }
                ],
                "signatures": [
                    {
                        "pub_key": {
                            "type": "tendermint/PubKeySecp256k1",
                            "value": "A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"
                        },
                        "signature": "4rE015+9PfMPmylMiPuE0OSCEE8vfgoMbH2AFTfyqU4t09kRNeH1fAV8c4yQ3CxB+0gUBySZrM0iygA3626v8w==",
                        "sequence": 123
                    }
                ]
            }
        }        
    )");
    EXPECT_EQ(hex(output.signature()), "e2b134d79fbd3df30f9b294c88fb84d0e482104f2f7e0a0c6c7d801537f2a94e2dd3d91135e1f57c057c738c90dc2c41fb4814072499accd22ca0037eb6eaff3");
}

TEST(THORChainSigner, SignJson) {
    auto inputJson = R"({"fee":{"amounts":[{"denom":"rune","amount":"200"}],"gas":"2000000"},"memo":"memo1234","messages":[{"sendCoinsMessage":{"fromAddress":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","toAddress":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn","amounts":[{"denom":"rune","amount":"50000000"}]}}]})";
    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");

    auto outputJson = THORChain::Signer::signJSON(inputJson, privateKey);

    assertJSONEqual(outputJson, R"(
        {
            "mode": "block",
            "tx": {
                "fee": {
                    "amount": [
                        {
                            "amount": "200",
                            "denom": "rune"
                        }
                    ],
                    "gas": "2000000"
                },
                "memo": "memo1234",
                "msg": [
                    {
                        "type": "thorchain/MsgSend",
                        "value": {
                            "amount": [
                                {
                                    "amount": "50000000",
                                    "denom": "rune"
                                }
                            ],
                            "from_address": "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
                            "to_address": "thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"
                        }
                    }
                ],
                "signatures": [
                    {
                        "pub_key": {
                            "type": "tendermint/PubKeySecp256k1",
                            "value": "A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"
                        },
                        "signature": "12AaNC0v51Rhz8rBf7V7rpI6oksREWrjzba3RK1v1NNlqZq62sG0aXWvStp9zZXe07Pp2FviFBAx+uqWsO30NQ==",
                        "sequence": 0
                    }
                ]
            }
        }
    )");
}
