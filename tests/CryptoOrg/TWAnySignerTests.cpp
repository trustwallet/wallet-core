// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>
#include "proto/Cosmos.pb.h"
#include "HexCoding.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWAnySignerCryptoorg, SignTx_DDCCE4) {
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
    amountOfTx->set_amount(100000000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("basecro");
    amountOfFee->set_amount(5000);

    auto privateKey = parse_hex("200e439e39cf1aad465ee3de6166247f914cbc0f823fc2dd48bf16dcd556f39d");
    input.set_private_key(privateKey.data(), privateKey.size());

    Cosmos::Proto::SigningOutput output;
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

    /// https://crypto.org/explorer/tx/DDCCE4052040B05914CADEFE78C0A75BE363AE39504E7EF6B2EDB8A9072AD44B
}
