// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "proto/Cosmos.pb.h"
#include "Cosmos/Address.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "TestUtilities.h"
#include "TrustWalletCore/TWAnySigner.h"

#include <gtest/gtest.h>
#include <google/protobuf/util/json_util.h>

namespace TW::Cosmos::tests {

TEST(CryptoorgSigner, SignTx_DDCCE4) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_account_number(125798);
    input.set_sequence(0);
    input.set_chain_id("crypto-org-chain-mainnet-1");

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address("cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0");
    message.set_to_address("cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus");
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("basecro");
    amountOfTx->set_amount("100000000");

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("basecro");
    amountOfFee->set_amount("5000");

    std::string json;
    google::protobuf::util::MessageToJsonString(input, &json);

    assertJSONEqual(json, R"(
        {
            "accountNumber": "125798",
            "chainId": "crypto-org-chain-mainnet-1",
            "fee": {
                "amounts": [
                    {
                        "denom": "basecro",
                        "amount": "5000"
                    }
                ],
                "gas": "200000"
            },
            "messages": [
                {
                    "sendCoinsMessage": {
                        "fromAddress": "cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0",
                        "toAddress": "cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus",
                        "amounts": [
                            {
                                "denom": "basecro",
                                "amount": "100000000"
                            }
                        ]
                    }
                }
            ]
        }
    )");

    auto privateKey = parse_hex("200e439e39cf1aad465ee3de6166247f914cbc0f823fc2dd48bf16dcd556f39d");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto output = Cosmos::Proto::SigningOutput();
    ANY_SIGN(input, TWCoinTypeCryptoOrg);

    assertJSONEqual(output.json(), R"(
        {
            "mode": "block",
            "tx": {
                "fee": {
                    "amount": [
                        {
                            "amount": "5000",
                            "denom": "basecro"
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
                                    "amount": "100000000",
                                    "denom": "basecro"
                                }
                            ],
                            "from_address": "cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0",
                            "to_address": "cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus"
                        }
                    }
                ],
                "signatures": [
                    {
                        "pub_key": {
                            "type": "tendermint/PubKeySecp256k1",
                            "value": "A4gxsGFiPn6L5Z2IjHEISkXI0IkwfL9exV3GLB171Wvj"
                        },
                        "signature": "5+5rSFFg0FE9cTklQWQHNktBDJsz7UCnMSgF0t0+gYcrIhEWUyTtibXaHZQbKAAaciJ1BkHXYREjU55VswByVg=="
                    }
                ]
            }
        }
    )");
    EXPECT_EQ(hex(output.signature()), "e7ee6b485160d0513d713925416407364b410c9b33ed40a7312805d2dd3e81872b2211165324ed89b5da1d941b28001a7222750641d7611123539e55b3007256");

    /// https://crypto.org/explorer/tx/DDCCE4052040B05914CADEFE78C0A75BE363AE39504E7EF6B2EDB8A9072AD44B
    /// curl -H 'Content-Type: application/json' --data-binary '{"mode":"block","tx":{"fee": ... }}' https://mainnet.crypto.org:1317/txs
}

TEST(CryptoorgSigner, SignJson) {
    auto inputJson = R"({"accountNumber":"125798","chainId":"crypto-org-chain-mainnet-1","fee":{"amounts":[{"denom":"basecro","amount":"5000"}],"gas":"200000"},"messages":[{"sendCoinsMessage":{"fromAddress":"cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0","toAddress":"cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus","amounts":[{"denom":"basecro","amount":"100000000"}]}}]})";
    auto privateKey = parse_hex("200e439e39cf1aad465ee3de6166247f914cbc0f823fc2dd48bf16dcd556f39d");

    auto outputJson = TW::anySignJSON(TWCoinTypeCryptoOrg, inputJson, privateKey);

    assertJSONEqual(outputJson, R"(
        {
            "mode": "block",
            "tx": {
                "fee": {
                    "amount": [
                        {
                            "amount": "5000",
                            "denom": "basecro"
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
                                    "amount": "100000000",
                                    "denom": "basecro"
                                }
                            ],
                            "from_address": "cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0",
                            "to_address": "cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus"
                        }
                    }
                ],
                "signatures": [
                    {
                        "pub_key": {
                            "type": "tendermint/PubKeySecp256k1",
                            "value": "A4gxsGFiPn6L5Z2IjHEISkXI0IkwfL9exV3GLB171Wvj"
                        },
                        "signature": "5+5rSFFg0FE9cTklQWQHNktBDJsz7UCnMSgF0t0+gYcrIhEWUyTtibXaHZQbKAAaciJ1BkHXYREjU55VswByVg=="
                    }
                ]
            }
        }
    )");
}

} // namespace TW::Cosmos::tests
