// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "algorithm/string.hpp"

#include "gtest/gtest.h"

namespace TW::tests {
    TEST(Algorithm, StringSplit) {
        auto splitted = TW::ssplit("0.0.1", '.');
        ASSERT_EQ(splitted.size(), 3uL);
        ASSERT_EQ(splitted[0], "0");
        ASSERT_EQ(splitted[1], "0");
        ASSERT_EQ(splitted[2], "1");
    }
}
