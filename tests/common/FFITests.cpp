// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include "TrustWalletCore/TWFFITest.h"

namespace TW::FFI::tests {

TEST(FFI, StringWithU8) {
    auto a = STRING("Hello");
    auto b = 97; // 'a'
    auto result = WRAPS(TWFFITestStringWithU8(a.get(), b));
    assertStringsEqual(result, "Helloa");
}

TEST(FFI, UnsignedSumU8) {
    auto a = 10;
    auto b = 20;
    auto result = TWFFITestUnsignedSumU8(a, b);
    ASSERT_EQ(result, 30);
}

TEST(FFI, SignedSumI8) {
    auto a = -10;
    auto b = 20;
    auto result = TWFFITestSignedSumI8(a, b);
    ASSERT_EQ(result, 10);
}

} // namespace TW::FFI::tests
