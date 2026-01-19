// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include  "Data.h"
#include  "rand.h"

#include <gtest/gtest.h>

using namespace TW;

TEST(Random, BufferResize) {
    Data emptyBuffer;
    Random::random_buffer(emptyBuffer, 32);
    EXPECT_EQ(emptyBuffer.size(), 32ul);

    Data biggerBuffer(256, 0);
    Random::random_buffer(biggerBuffer, 64);
    EXPECT_EQ(biggerBuffer.size(), 64ul);
}
