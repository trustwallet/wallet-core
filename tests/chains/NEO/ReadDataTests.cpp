// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "NEO/ReadData.h"

#include <gtest/gtest.h>

#include "TestUtilities.h"

namespace TW::NEO::tests {

using namespace std;

TEST(NEOReadData, readBytes) {
    EXPECT_EXCEPTION(TW::readBytes(Data{}, 10), "Data::Cannot read enough bytes!");
}

TEST(NEOReadData, readVar) {
    Data from{0xfe, 0x00, 0x00, 0x00, 0x01};
    int64_t max = 0;
    EXPECT_EXCEPTION(TW::readVar(from, 0, max), "ReadData::ReadVarInt error: Too huge value! FormatException");
    
    max = INT64_MAX;
    ASSERT_EQ(TW::readVar(from, 0, max), 0x1000000);

    from = {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    ASSERT_EQ(TW::readVar(from, 0, max), 0x100000000000000);
}

} // namespace TW::NEO::tests
