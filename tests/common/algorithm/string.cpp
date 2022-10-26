//
// Created by Roman Sztergbaum on 26/10/2022.
//

#include "algorithm/string.h"

#include "gtest/gtest.h"

namespace TW::tests {
    TEST(Algorithm, StringSplit) {
        auto splitted = TW::split("0.0.1", '.');
        ASSERT_EQ(splitted.size(), 3uL);
        ASSERT_EQ(splitted[0], "0");
        ASSERT_EQ(splitted[1], "0");
        ASSERT_EQ(splitted[2], "1");
    }
}
