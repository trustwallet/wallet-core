// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "proto/Cosmos.pb.h"
#include "MAYAChain/Signer.h"
#include "HexCoding.h"
#include "Bech32Address.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

using namespace TW;


TEST(MAYAChainSigner, SignTx_Protobuf_7E480F) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_signing_mode(Cosmos::Proto::Protobuf);
    input.set_chain_id("mayachain-mainnet-v1");
    input.set_account_number(593);
    input.set_sequence(21);
    input.set_memo("");
    auto msg = input.add_messages();
    auto& message = *msg->mutable_mayachain_send_message();
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
                    "mayachainSendMessage": {
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
    auto output = MAYAChain::Signer::sign(input);

    // https://viewblock.io/mayachain/tx/7E480FA163F6C6AFA17593F214C7BBC218F69AE3BC72366E39042AF381BFE105
    // curl -H 'Content-Type: application/json' --data-binary '{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"ClIKUAoO..89g="}' https://<mayanode>/cosmos/tx/v1beta1/txs
    assertJSONEqual(output.serialized(), R"(
        {
            "mode": "BROADCAST_MODE_BLOCK",
            "tx_bytes": "ClMKUQoOL3R5cGVzLk1zZ1NlbmQSPwoU5xsDhaVQJzA/jXPGFD4CCjEP4KESFBJiiFvZaJ3ywITKWJv/MfYksH40GhEKBWNhY2FvEggzODAwMDAwMBJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiED7Zl+OWz0KS9fzlpCu6QVmczV2W4xMVSnyepwSd4xfHcSBAoCCAEYFRITCgwKBWNhY2FvEgMyMDAQoMuYARpAXaQ5ZKFsnpi1n27GIuAAsuOzuuD7hlepUt6QgQ1QwNwqYYhRoOsfqSXq3Uu9aTCKWWP9XDh0Nylz/uh/ex7rLg=="
        }
    )");

    EXPECT_EQ(hex(output.signature()), "5da43964a16c9e98b59f6ec622e000b2e3b3bae0fb8657a952de90810d50c0dc2a618851a0eb1fa925eadd4bbd69308a5963fd5c3874372973fee87f7b1eeb2e");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error(), "");
}

TEST(MAYAChainSigner, SignTx_Json_Deprecated) {
    auto input = Cosmos::Proto::SigningInput();
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
                        "signature": "CdNOWn9lMacKe9raiJdm2S/GjSNKG19NHUOLBrgJCf5b/RO3+6IHTFFWVQMAh5D3sKZUDA39ZWrCX8Pe1+UIVg=="
                    }
                ]
            }
        }
    )");
    EXPECT_EQ(hex(output.signature()), "09d34e5a7f6531a70a7bdada889766d92fc68d234a1b5f4d1d438b06b80909fe5bfd13b7fba2074c51565503008790f7b0a6540c0dfd656ac25fc3ded7e50856");
}

TEST(MAYAChainSigner, SignJson) {
    auto inputJson = R"({"fee":{"amounts":[{"denom":"cacao","amount":"200"}],"gas":"2000000"},"memo":"memo1234","messages":[{"sendCoinsMessage":{"fromAddress":"maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","toAddress":"maya1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn","amounts":[{"denom":"cacao","amount":"50000000"}]}}]})";
    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");

    auto outputJson = MAYAChain::Signer::signJSON(inputJson, privateKey);

    EXPECT_EQ(R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"200","denom":"cacao"}],"gas":"2000000"},"memo":"memo1234","msg":[{"type":"mayachain/MsgSend","value":{"amount":[{"amount":"50000000","denom":"cacao"}],"from_address":"maya1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","to_address":"maya1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"CdNOWn9lMacKe9raiJdm2S/GjSNKG19NHUOLBrgJCf5b/RO3+6IHTFFWVQMAh5D3sKZUDA39ZWrCX8Pe1+UIVg=="}]}})", outputJson);
}
