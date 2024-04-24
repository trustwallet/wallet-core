// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "NumericLiteral.h"

#include <gtest/gtest.h>

TEST(UzLitteralOperator, SizetEquality) {
    [[maybe_unused]] auto size = 42_uz;
    static_assert(std::is_same_v<decltype(size), std::size_t>);
}