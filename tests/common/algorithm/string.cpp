// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
