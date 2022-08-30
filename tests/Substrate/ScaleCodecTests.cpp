// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Substrate/ScaleCodec.h"
#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

namespace TW::Substrate::tests {

    TEST(SubstrateScaleCodec, encodeEra) {
        uint64_t block = 4246319;
        uint64_t period = 64;
        auto data = encodeEra(block, period);
        EXPECT_EQ(hex(data), "f502");
    }

} // namespace TW::Substrate::tests