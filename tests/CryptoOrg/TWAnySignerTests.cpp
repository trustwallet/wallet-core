// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>
#include "proto/Cosmos.pb.h"
#include "HexCoding.h"
#include "Base64.h"
#include "Data.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;


const auto Address1 = "cro1ctwtcwpgksky988dhth6jslxveumgu0d45zgf0";
const auto Address2 = "cro1xpahy6c7wldxacv6ld99h435mhvfnsup24vcus";
const auto PrivateKey1 = "200e439e39cf1aad465ee3de6166247f914cbc0f823fc2dd48bf16dcd556f39d";

TEST(TWAnySignerCryptoorg, SignTx_Proto_BCB213) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_signing_mode(Cosmos::Proto::Protobuf);
    input.set_account_number(125798);
    input.set_sequence(2);
    input.set_chain_id("crypto-org-chain-mainnet-1");

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(Address1);
    message.set_to_address(Address2);
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("basecro");
    amountOfTx->set_amount(50000000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("basecro");
    amountOfFee->set_amount(5000);

    auto privateKey = parse_hex(PrivateKey1);
    input.set_private_key(privateKey.data(), privateKey.size());

    Cosmos::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeCryptoOrg);

    // https://crypto.org/explorer/tx/BCB213B0A121F0CF11BECCF52475F1C8328D6070F3CFDA9E14C42E6DB30E847E
    EXPECT_EQ(hex(output.serialized()), "0a90010a8d010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e64126d0a2a63726f3163747774637770676b736b7939383864687468366a736c787665756d6775306434357a676630122a63726f317870616879366337776c6478616376366c643939683433356d6876666e7375703234766375731a130a076261736563726f1208353030303030303012690a500a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a21038831b061623e7e8be59d888c71084a45c8d089307cbf5ec55dc62c1d7bd56be312040a020801180212150a0f0a076261736563726f12043530303010c09a0c1a4007312bdc64eabebd826cfed5459a0a763136e5cf5d9769e7d61ca8a3c913977a7e9f882024c13b0776aecf0c880a5c7fc90d4ab6d9ea8102c5c19001dc45d122");
    EXPECT_EQ(output.serialized_base64(), "CpABCo0BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm0KKmNybzFjdHd0Y3dwZ2tza3k5ODhkaHRoNmpzbHh2ZXVtZ3UwZDQ1emdmMBIqY3JvMXhwYWh5NmM3d2xkeGFjdjZsZDk5aDQzNW1odmZuc3VwMjR2Y3VzGhMKB2Jhc2Vjcm8SCDUwMDAwMDAwEmkKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQOIMbBhYj5+i+WdiIxxCEpFyNCJMHy/XsVdxiwde9Vr4xIECgIIARgCEhUKDwoHYmFzZWNybxIENTAwMBDAmgwaQAcxK9xk6r69gmz+1UWaCnYxNuXPXZdp59YcqKPJE5d6fp+IICTBOwd2rs8MiApcf8kNSrbZ6oECxcGQAdxF0SI=");
    EXPECT_EQ(hex(output.signature()), "5d75175d9413dd5abcdcbd3d04fe81e0b417853f636f79ad796eb2e379312b05324d7a2d2075dd6c864bff722403566119651c792b76728a72ab678e10a7a852");
    EXPECT_EQ(output.json(), "");
}

TEST(TWAnySignerCryptoorg, SignTx_Json_DDCCE4) {
    auto input = Cosmos::Proto::SigningInput();
    input.set_account_number(125798);
    input.set_sequence(0);
    input.set_chain_id("crypto-org-chain-mainnet-1");

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(Address1);
    message.set_to_address(Address2);
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("basecro");
    amountOfTx->set_amount(100000000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("basecro");
    amountOfFee->set_amount(5000);

    auto privateKey = parse_hex(PrivateKey1);
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
    EXPECT_EQ(hex(output.signature()), "e7ee6b485160d0513d713925416407364b410c9b33ed40a7312805d2dd3e81872b2211165324ed89b5da1d941b28001a7222750641d7611123539e55b3007256");
    EXPECT_EQ(output.serialized(), "");
    EXPECT_EQ(output.serialized_base64(), "");

    /// https://crypto.org/explorer/tx/DDCCE4052040B05914CADEFE78C0A75BE363AE39504E7EF6B2EDB8A9072AD44B
}
