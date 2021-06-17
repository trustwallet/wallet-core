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
    auto structType = WRAPS(TWStringCreateWithUTF8Bytes("Person"));
    auto valueJson = WRAPS(TWStringCreateWithUTF8Bytes(
        R"(
            {
                "name": "Cow",
                "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"
            }
        )"));
    auto typesJson = WRAPS(TWStringCreateWithUTF8Bytes(
        R"(
            [
                {"Person":
                    [
                        {
                            "name": "name",
                            "type": "string"
                        },
                        {
                            "name": "wallet",
                            "type": "address"
                        }
                    ]
                }
            ]
        )"));

    auto hash = WRAPD(TWEthereumAbiStructHashStruct(structType.get(), valueJson.get(), typesJson.get()));

    EXPECT_EQ(hex(TW::data(TWDataBytes(hash.get()), TWDataSize(hash.get()))), "fc71e5fa27ff56c350aa531bc129ebdf613b772b6604664f5d8dbe21b85eb0c8");
}
