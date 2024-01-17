// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include <TrustWalletCore/TWAnyAddress.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

namespace TW::Komodo::tests {

TEST(TWKomodo, Address) {
    auto string = STRING("RALiENnMMjyubc38hM31h6oicPsuWdAMYg");
    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeKomodo));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string.get(), string2.get()));
    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "0ba28b3eebfe1d39dab038324be2c66090ee21a3");
}

} // namespace TW::Komodo::tests
