// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Result.h"
#include "Data.h"

#include <gtest/gtest.h>

namespace TW::tests {

TEST(Result, MoveConstructSuccess) {
    auto source = Result<Data>::success(Data{1, 2, 3});
    const Result<Data> moved(std::move(source));
    ASSERT_TRUE(moved.isSuccess());
    EXPECT_EQ(moved.payload(), (Data{1, 2, 3}));
}

TEST(Result, MoveConstructFailure) {
    auto source = Result<Data>::failure("error");
    const Result<Data> moved(std::move(source));
    ASSERT_TRUE(moved.isFailure());
    EXPECT_EQ(moved.error(), "error");
}

} // namespace TW::tests
