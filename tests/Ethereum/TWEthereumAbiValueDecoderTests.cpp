// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiValueDecoder.h>

#include "Data.h"
#include "HexCoding.h"
#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace std;

TEST(TWEthereumAbiValueDecoder, decodeUInt256) {
    const char * expected = "10020405371567";
    auto inputs = vector<string>{
        "091d0eb3e2af",
        "0000000000000000000000000000000000000000000000000000091d0eb3e2af",
        "0000000000000000000000000000000000000000000000000000091d0eb3e2af0000000000000000000000000000000000000000000000000000000000000000",
        "0000000000000000000000000000000000000000000000000000091d0eb3e2af00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    };
    for (auto &input: inputs) {
        auto data =  WRAPD(TWDataCreateWithHexString(STRING(input.c_str()).get()));
        auto result = WRAPS(TWEthereumAbiValueDecoderDecodeUInt256(data.get()));
        assertStringsEqual(result, expected);
    }
}
