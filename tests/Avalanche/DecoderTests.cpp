// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include "Avalanche/Decoder.h"
#include "Data.h"
#include "HexCoding.h"

#include <gtest/gtest.h>
#include <iostream>

using namespace TW;
using namespace TW::Avalanche;

TEST(Avalanche, Decoder) {
    const auto data = parse_hex("0x0000da7ba5f0c8cc5d1c14c2279ffe77abe7c22d1e90f2b0575843d8458e82c7e5320000000021e67317cbc4be2aeb00677ad6462778a8f52274b9d605df2591b23027a87dff00000007000000003b7c458000000000000000000000000100000001b7bce5a6b2263881816034938606a12c014f8bf7363e9c39");

    auto decoded = decodeUTXO(data);

    auto expected = R"|(
        {
            "addresses": ["b7bce5a6b2263881816034938606a12c014f8bf7"],
            "amount": 998000000,
            "asset": "21e67317cbc4be2aeb00677ad6462778a8f52274b9d605df2591b23027a87dff",
            "codec": 0,
            "hash": "da7ba5f0c8cc5d1c14c2279ffe77abe7c22d1e90f2b0575843d8458e82c7e532",
            "index": 0,
            "locktime": 0,
            "threshold": 1,
            "type": 7
        }
    )|";

    assertJSONEqual(decoded.value(), expected);
}
