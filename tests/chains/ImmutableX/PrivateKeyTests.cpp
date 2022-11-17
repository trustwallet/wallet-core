// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ImmutableX/PrivateKey.h"
#include <gtest/gtest.h>

namespace TW::ImmutableX::tests {

TEST(ImmutableX, PrivateKey) {
    int256_t biggerNumber = internal::gStarkCurveN + 1;
    ASSERT_ANY_THROW([[maybe_unused]] auto res = PrivateKey(biggerNumber));
    ASSERT_ANY_THROW([[maybe_unused]] auto res = PrivateKey(uint256_t(0)));

    int256_t inRange = internal::gStarkCurveN - 1;
    auto pk = PrivateKey(inRange);
    ASSERT_EQ(pk.mNumber, inRange);
}

TEST(ImmutableX, PrivateKeyHex) {
    ASSERT_ANY_THROW([[maybe_unused]] auto res = PrivateKey("Invalid"));
    int256_t inRange = internal::gStarkCurveN - 1;
    std::stringstream ss;
    ss << std::hex << inRange;
    auto pk = PrivateKey(ss.str());
    ASSERT_EQ(pk.mNumber, inRange);
}

}
