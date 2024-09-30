// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "memory/memzero_wrapper.h"

#include "gtest/gtest.h"

struct int_wrapper {
    int value;
};

TEST(Memory, Memzero) {
    int_wrapper obj{.value = 42};
    TW::memzero(&obj);
    ASSERT_EQ(obj.value, 0);
    obj.value = 42;
    ASSERT_EQ(obj.value, 42);
    TW::memzero(&obj, sizeof(int_wrapper));
    ASSERT_EQ(obj.value, 0);
}
