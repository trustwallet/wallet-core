// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "operators/equality_comparable.h"

#include "gtest/gtest.h"

namespace TW::operators::tests {

struct Amount : equality_comparable<Amount> {
    int value;
    friend bool operator==(const Amount& lhs, const Amount& rhs) { return lhs.value == rhs.value; }
};

TEST(Operators, EqualityComparable) {
    ASSERT_TRUE(Amount{.value = 1} != Amount{.value = 2});
    ASSERT_TRUE(Amount{.value = 1} == Amount{.value = 1});
}

} // namespace TW::operators::tests
