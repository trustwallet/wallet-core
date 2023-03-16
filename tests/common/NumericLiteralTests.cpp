// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "NumericLiteral.h"

#include <gtest/gtest.h>

TEST(UzLitteralOperator, SizetEquality) {
    [[maybe_unused]] auto size = 42_uz;
    static_assert(std::is_same_v<decltype(size), std::size_t>);
}