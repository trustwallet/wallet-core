// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
