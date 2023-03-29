// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
