// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "proto/Cosmos.pb.h"
#include "THORChain/Signer.h"
#include "MAYAChain/Signer.h"
#include "HexCoding.h"
#include "Bech32Address.h"
#include "TestUtilities.h"

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

    auto output = THORChain::Signer::sign(input);

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


TEST(THORChainSigner, SignTx_Maya_Protobuf_7E480F) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_signing_mode(Cosmos::Proto::Protobuf);
    input.set_chain_id("mayachain-mainnet-v1");
    input.set_account_number(593);
    input.set_sequence(21);
    input.set_memo("");
    auto msg = input.add_messages();
    auto& message = *msg->mutable_thorchain_send_message();
    Bech32Address fromAddress("maya");
    EXPECT_TRUE(Bech32Address::decode("maya1uuds8pd92qnnq0udw0rpg0szpgcslc9prlsqk8", fromAddress, "maya"));
    Bech32Address toAddress("maya");
    EXPECT_TRUE(Bech32Address::decode("maya1zf3gsk7edzwl9syyefvfhle37cjtql35n6emmw", toAddress, "maya"));
    message.set_from_address(std::string(fromAddress.getKeyHash().begin(), fromAddress.getKeyHash().end()));
    message.set_to_address(std::string(toAddress.getKeyHash().begin(), toAddress.getKeyHash().end()));
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("cacao");
    amountOfTx->set_amount("38000000");
    auto& fee = *input.mutable_fee();
    fee.set_gas(2500000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cacao");
    amountOfFee->set_amount("200");
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

    auto output = THORChain::Signer::sign(input);

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

TEST(THORChainSigner, SignTx_Maya_Protobuf_7E480F) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_signing_mode(Cosmos::Proto::Protobuf);
    input.set_chain_id("mayachain-mainnet-v1");
    input.set_account_number(593);
    input.set_sequence(21);
    input.set_memo("");
    auto msg = input.add_messages();
    auto& message = *msg->mutable_thorchain_send_message();
    Bech32Address fromAddress("maya");
    EXPECT_TRUE(Bech32Address::decode("maya1uuds8pd92qnnq0udw0rpg0szpgcslc9prlsqk8", fromAddress, "maya"));
    Bech32Address toAddress("maya");
    EXPECT_TRUE(Bech32Address::decode("maya1zf3gsk7edzwl9syyefvfhle37cjtql35n6emmw", toAddress, "maya"));
    message.set_from_address(std::string(fromAddress.getKeyHash().begin(), fromAddress.getKeyHash().end()));
    message.set_to_address(std::string(toAddress.getKeyHash().begin(), toAddress.getKeyHash().end()));
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("cacao");
    amountOfTx->set_amount("38000000");
    auto& fee = *input.mutable_fee();
    fee.set_gas(2500000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cacao");
    amountOfFee->set_amount("200");
    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    assertJSONEqual(json, R"(
        {
            "accountNumber": "593",
            "chainId": "mayachain-mainnet-v1",
            "fee": {
                "amounts": [
                    {
                        "amount": "200",
                        "denom": "cacao"
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
                                "denom": "cacao"
                            }
                        ],
                        "fromAddress": "5xsDhaVQJzA/jXPGFD4CCjEP4KE=",
                        "toAddress": "EmKIW9lonfLAhMpYm/8x9iSwfjQ="
                    }
                }
            ],
            "sequence": "21",
            "signingMode": "Protobuf"
        }
    )");
    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");
    input.set_private_key(privateKey.data(), privateKey.size());
    auto output = THORChain::Signer::sign(input);

    assertJSONEqual(output.serialized(), R"(
        {
            "mode": "BROADCAST_MODE_BLOCK",
            "tx_bytes": "ClMKUQoOL3R5cGVzLk1zZ1NlbmQSPwoU5xsDhaVQJzA/jXPGFD4CCjEP4KESFBJiiFvZaJ3ywITKWJv/MfYksH40GhEKBWNhY2FvEggzODAwMDAwMBJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiED7Zl+OWz0KS9fzlpCu6QVmczV2W4xMVSnyepwSd4xfHcSBAoCCAEYFRITCgwKBWNhY2FvEgMyMDAQoMuYARpAXaQ5ZKFsnpi1n27GIuAAsuOzuuD7hlepUt6QgQ1QwNwqYYhRoOsfqSXq3Uu9aTCKWWP9XDh0Nylz/uh/ex7rLg=="
        }
    )");

    EXPECT_EQ(hex(output.signature()), "5da43964a16c9e98b59f6ec622e000b2e3b3bae0fb8657a952de90810d50c0dc2a618851a0eb1fa925eadd4bbd69308a5963fd5c3874372973fee87f7b1eeb2e");
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
                        "signature": "12AaNC0v51Rhz8rBf7V7rpI6oksREWrjzba3RK1v1NNlqZq62sG0aXWvStp9zZXe07Pp2FviFBAx+uqWsO30NQ=="
                    }
                ]
            }
        }
    )");
    EXPECT_EQ(hex(output.signature()), "d7601a342d2fe75461cfcac17fb57bae923aa24b11116ae3cdb6b744ad6fd4d365a99abadac1b46975af4ada7dcd95ded3b3e9d85be2141031faea96b0edf435");
}

TEST(MAYAChainSigner, SignTx_Maya_Json_Deprecated) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_chain_id("mayachain");
    input.set_memo("memo1234");

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address("maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r");
    message.set_to_address("maya1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn");
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("cacao");
    amountOfTx->set_amount("50000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(2000000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cacao");
    amountOfFee->set_amount("200");

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    assertJSONEqual(json, R"(
        {
            "chainId": "mayachain",
            "fee": {
                "amounts": [
                    {
                        "denom": "cacao",
                        "amount": "200"
                    }
                ],
                "gas": "2000000"
            },
            "memo": "memo1234",
            "messages": [
                {
                    "sendCoinsMessage": {
                        "fromAddress": "maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
                        "toAddress": "maya1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn",
                        "amounts": [
                            {
                                "denom": "cacao",
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

    auto output = MAYAChain::Signer::sign(input);

    assertJSONEqual(output.json(), R"(
        {
            "mode": "block",
            "tx": {
                "fee": {
                    "amount": [
                        {
                            "amount": "200",
                            "denom": "cacao"
                        }
                    ],
                    "gas": "2000000"
                },
                "memo": "memo1234",
                "msg": [
                    {
                        "type": "mayachain/MsgSend",
                        "value": {
                            "amount": [
                                {
                                    "amount": "50000000",
                                    "denom": "cacao"
                                }
                            ],
                            "from_address": "maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r",
                            "to_address": "maya1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"
                        }
                    }
                ],
                "signatures": [
                    {
                        "pub_key": {
                            "type": "tendermint/PubKeySecp256k1",
                            "value": "A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"
                        },
                        "signature": "6HvrKL5Up8BzLomkIhSvzOSnRoYQZBSXgm+i+5xJHs0YZdr+n2V1BsUvn/gfCOERRX6bIrXcz+AYI1alfn7CKg=="
                    }
                ]
            }
        }
    )");
    EXPECT_EQ(hex(output.signature()), "e87beb28be54a7c0732e89a42214afcce4a7468610641497826fa2fb9c491ecd1865dafe9f657506c52f9ff81f08e111457e9b22b5dccfe0182356a57e7ec22a");
}

TEST(THORChainSigner, SignJson) {
    auto inputJson = R"({"fee":{"amounts":[{"denom":"rune","amount":"200"}],"gas":"2000000"},"memo":"memo1234","messages":[{"sendCoinsMessage":{"fromAddress":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","toAddress":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn","amounts":[{"denom":"rune","amount":"50000000"}]}}]})";
    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");

    auto outputJson = THORChain::Signer::signJSON(inputJson, privateKey);

    EXPECT_EQ(R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"rune"}],"gas":"2000000"},"memo":"memo1234","msg":[{"type":"thorchain/MsgSend","value":{"amount":[{"amount":"50000000","denom":"rune"}],"from_address":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","to_address":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"12AaNC0v51Rhz8rBf7V7rpI6oksREWrjzba3RK1v1NNlqZq62sG0aXWvStp9zZXe07Pp2FviFBAx+uqWsO30NQ=="}]}})", outputJson);
}

TEST(THORChainSigner, SignJson_Maya) {
    auto inputJson = R"({"fee":{"amounts":[{"denom":"cacao","amount":"200"}],"gas":"2000000"},"memo":"memo1234","messages":[{"sendCoinsMessage":{"fromAddress":"maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","toAddress":"maya1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn","amounts":[{"denom":"cacao","amount":"50000000"}]}}]})";
    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");

    auto outputJson = MAYAChain::Signer::signJSON(inputJson, privateKey);

    EXPECT_EQ(R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"cacao"}],"gas":"2000000"},"memo":"memo1234","msg":[{"type":"mayachain/MsgSend","value":{"amount":[{"amount":"50000000","denom":"cacao"}],"from_address":"maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","to_address":"maya1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"CdNOWn9lMacKe9raiJdm2S/GjSNKG19NHUOLBrgJCf5b/RO3+6IHTFFWVQMAh5D3sKZUDA39ZWrCX8Pe1+UIVg=="}]}})", outputJson);
}