// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "proto/Cosmos.pb.h"
#include "Coin.h"
#include "HexCoding.h"
#include "Bech32Address.h"
#include "TestUtilities.h"
#include "TrustWalletCore/TWAnySigner.h"
#include "TrustWalletCore/TWCoinType.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

using namespace TW;


TEST(THORChainSigner, SignTx_Protobuf_7E480F) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_signing_mode(Cosmos::Proto::Protobuf);
    input.set_chain_id("thorchain-mainnet-v1");
    input.set_account_number(593);
    input.set_sequence(21);
    input.set_memo("");

    auto msg = input.add_messages();
    auto& message = *msg->mutable_thorchain_send_message();
    Bech32Address fromAddress("thor");
    EXPECT_TRUE(Bech32Address::decode("thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r", fromAddress, "thor"));
    Bech32Address toAddress("thor");
    EXPECT_TRUE(Bech32Address::decode("thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn", toAddress, "thor"));
    message.set_from_address(std::string(fromAddress.getKeyHash().begin(), fromAddress.getKeyHash().end()));
    message.set_to_address(std::string(toAddress.getKeyHash().begin(), toAddress.getKeyHash().end()));
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("rune");
    amountOfTx->set_amount("38000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(2500000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("rune");
    amountOfFee->set_amount("200");

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    assertJSONEqual(json, R"(
        {
            "accountNumber": "593",
            "chainId": "thorchain-mainnet-v1",
            "fee": {
                "amounts": [
                    {
                        "amount": "200",
                        "denom": "rune"
                    }
                ],
                "gas": "2500000"
            },
            "messages": [
                {
                    "thorchainSendMessage": {
                        "amounts": [
                            {
                                "amount": "38000000",
                                "denom": "rune"
                            }
                        ],
                        "fromAddress": "FSLnZ9tusZcIsAOAKb+9YHvJvQ4=",
                        "toAddress": "yoZFn7AFUcffQlQMXnhpGSyDOts="
                    }
                }
            ],
            "sequence": "21",
            "signingMode": "Protobuf"
        }
    )");

    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Cosmos::Proto::SigningOutput();
    ANY_SIGN(input, TWCoinTypeTHORChain);

    // https://viewblock.io/thorchain/tx/7E480FA163F6C6AFA17593F214C7BBC218F69AE3BC72366E39042AF381BFE105
    // curl -H 'Content-Type: application/json' --data-binary '{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"ClIKUAoO..89g="}' https://<thornode>/cosmos/tx/v1beta1/txs
    assertJSONEqual(output.serialized(), R"(
        {
            "mode": "BROADCAST_MODE_BLOCK",
            "tx_bytes": "ClIKUAoOL3R5cGVzLk1zZ1NlbmQSPgoUFSLnZ9tusZcIsAOAKb+9YHvJvQ4SFMqGRZ+wBVHH30JUDF54aRksgzrbGhAKBHJ1bmUSCDM4MDAwMDAwEmYKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQPtmX45bPQpL1/OWkK7pBWZzNXZbjExVKfJ6nBJ3jF8dxIECgIIARgVEhIKCwoEcnVuZRIDMjAwEKDLmAEaQKZtS3ATa26OOGvqdKm14ZbHeNfkPtIajXi5MkZ5XaX2SWOeX+YnCPZ9TxF9Jj5cVIo71m55xq4hVL3yDbRe89g="
        }
    )");
    EXPECT_EQ(hex(output.signature()), "a66d4b70136b6e8e386bea74a9b5e196c778d7e43ed21a8d78b93246795da5f649639e5fe62708f67d4f117d263e5c548a3bd66e79c6ae2154bdf20db45ef3d8");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error_message(), "");
}

TEST(THORChainSigner, SignTx_MsgDeposit) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_signing_mode(Cosmos::Proto::Protobuf);
    input.set_chain_id("thorchain-mainnet-v1");
    input.set_account_number(75247);
    input.set_sequence(7);

    auto msg = input.add_messages();
    auto& message = *msg->mutable_thorchain_deposit_message();

    message.set_memo("=:DOGE.DOGE:DNhRF1h8J4ZnB1bxp9kaqhVLYetkx1nSJ5::tr:0");
    Bech32Address signerAddress("thor");
    EXPECT_TRUE(Bech32Address::decode("thor14j5lwl8ulexrqp5x39kmkctv2937694z3jn2dz", signerAddress, "thor"));
    message.set_signer(std::string(signerAddress.getKeyHash().begin(), signerAddress.getKeyHash().end()));

    auto& coins = *message.add_coins();
    coins.set_amount("150000000");
    coins.set_decimals(0);

    auto& asset = *coins.mutable_asset();
    asset.set_chain("THOR");
    asset.set_symbol("RUNE");
    asset.set_ticker("RUNE");

    auto& fee = *input.mutable_fee();
    fee.set_gas(50000000);

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    assertJSONEqual(json, R"(
        {
            "accountNumber": "75247",
            "chainId": "thorchain-mainnet-v1",
            "fee": {
                "gas": "50000000"
            },
            "messages": [
                {
                    "thorchainDepositMessage": {
                        "coins": [
                            {
                                "amount": "150000000",
                                "asset": {
                                    "chain": "THOR",
                                    "symbol": "RUNE",
                                    "ticker": "RUNE"
                                }
                            }
                        ],
                        "memo": "=:DOGE.DOGE:DNhRF1h8J4ZnB1bxp9kaqhVLYetkx1nSJ5::tr:0",
                        "signer": "rKn3fPz+TDAGholtu2FsUWPtFqI="
                    }
                }
            ],
            "sequence": "7",
            "signingMode": "Protobuf"
        }
    )");

    auto privateKey = parse_hex("2659e41d54ebd449d68b9d58510d8eeeb837ee00d6ecc760b7a731238d8c3113");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Cosmos::Proto::SigningOutput();
    ANY_SIGN(input, TWCoinTypeTHORChain);

    // https://viewblock.io/thorchain/tx/0162213E7F9D85965B1C57FA3BF9603C655B542F358318303A7B00661AE42510
    // curl -H 'Content-Type: application/json' --data-binary '{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CoUBCoIB..hiw="}' https://<thornode>/cosmos/tx/v1beta1/txs
    assertJSONEqual(output.serialized(), R"(
        {
            "mode": "BROADCAST_MODE_BLOCK",
            "tx_bytes": "CoUBCoIBChEvdHlwZXMuTXNnRGVwb3NpdBJtCh8KEgoEVEhPUhIEUlVORRoEUlVORRIJMTUwMDAwMDAwEjQ9OkRPR0UuRE9HRTpETmhSRjFoOEo0Wm5CMWJ4cDlrYXFoVkxZZXRreDFuU0o1Ojp0cjowGhSsqfd8/P5MMAaGiW27YWxRY+0WohJZClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDuZVDlIFW3DtSEBa6aUBJ0DrQHlQ+2g7lIt5ekAM25SkSBAoCCAEYBxIFEIDh6xcaQAxKMZMKbM8gdLwn23GDXfbwyCkgqWzFMFlnrqFm0u54F8T32wmsoJQAdoLIyOskYmi7nb1rhryfabeeULwRhiw="
        }
    )");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error_message(), "");
}

TEST(THORChainSigner, SignTx_Json_Deprecated) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_memo("memo1234");

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address("thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r");
    message.set_to_address("thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn");
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("rune");
    amountOfTx->set_amount("50000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(2000000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("rune");
    amountOfFee->set_amount("200");

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
            ]
        }
    )");

    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Cosmos::Proto::SigningOutput();
    ANY_SIGN(input, TWCoinTypeTHORChain);

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
                        "signature": "12AaNC0v51Rhz8rBf7V7rpI6oksREWrjzba3RK1v1NNlqZq62sG0aXWvStp9zZXe07Pp2FviFBAx+uqWsO30NQ=="
                    }
                ]
            }
        }
    )");
    EXPECT_EQ(hex(output.signature()), "d7601a342d2fe75461cfcac17fb57bae923aa24b11116ae3cdb6b744ad6fd4d365a99abadac1b46975af4ada7dcd95ded3b3e9d85be2141031faea96b0edf435");
}

TEST(THORChainSigner, SignJson) {
    auto inputJson = R"({"fee":{"amounts":[{"denom":"rune","amount":"200"}],"gas":"2000000"},"memo":"memo1234","messages":[{"sendCoinsMessage":{"fromAddress":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","toAddress":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn","amounts":[{"denom":"rune","amount":"50000000"}]}}]})";
    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");

    auto outputJson = TW::anySignJSON(TWCoinTypeTHORChain, inputJson, privateKey);

    EXPECT_EQ(R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"rune"}],"gas":"2000000"},"memo":"memo1234","msg":[{"type":"thorchain/MsgSend","value":{"amount":[{"amount":"50000000","denom":"rune"}],"from_address":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","to_address":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"12AaNC0v51Rhz8rBf7V7rpI6oksREWrjzba3RK1v1NNlqZq62sG0aXWvStp9zZXe07Pp2FviFBAx+uqWsO30NQ=="}]}})", outputJson);
}
