// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiStruct.h>

#include <Data.h>
#include <HexCoding.h>

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(TWEthereumAbiStruct, hashStruct) {
    auto structType = WRAPS(TWStringCreateWithUTF8Bytes("Mail"));
    auto valueJson = WRAPS(TWStringCreateWithUTF8Bytes(
        R"(
            {
                "from": {
                    "name": "Cow",
                    "wallets": [
                        "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826",
                        "DeaDbeefdEAdbeefdEadbEEFdeadbeEFdEaDbeeF"
                    ]
                },
                "to": [
                    {
                        "name": "Bob",
                        "wallets": [
                            "bBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB",
                            "B0BdaBea57B0BDABeA57b0bdABEA57b0BDabEa57",
                            "B0B0b0b0b0b0B000000000000000000000000000"
                        ]
                    }
                ],
                "contents": "Hello, Bob!"
            }
        )"));
    auto typesJson = WRAPS(TWStringCreateWithUTF8Bytes(
        R"(
            [
                {
                    "Person": [
                        {"name": "name", "type": "string"},
                        {"name": "wallets", "type": "address[]"}
                    ]
                },
                {
                    "Mail": [
                        {"name": "from", "type": "Person"},
                        {"name": "to", "type": "Person[]"},
                        {"name": "contents", "type": "string"}
                    ]
                }
            ]
        )"));
    auto hash = WRAPD(TWEthereumAbiStructHashStruct(structType.get(), valueJson.get(), typesJson.get()));

    EXPECT_EQ(
        hex(TW::data(TWDataBytes(hash.get()), TWDataSize(hash.get()))),
        "eb4221181ff3f1a83ea7313993ca9218496e424604ba9492bb4052c03d5c3df8"
    );
}
