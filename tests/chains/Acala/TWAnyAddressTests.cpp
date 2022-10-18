// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include "HexCoding.h"
#include "Hash.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Polkadot::tests {

inline constexpr uint32_t acalaPrefix{10};

TEST(TWAcalaAnyAddress, IsValid) {
    EXPECT_TRUE(TWAnyAddressIsValidSS58(STRING("212ywJGVK2Nxnt5bjKXVHi4YY7FCFd4rVvhyt95CjpeHGZee").get(), TWCoinTypePolkadot, acalaPrefix));
    EXPECT_FALSE(TWAnyAddressIsValidSS58(STRING("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu").get(), TWCoinTypePolkadot, acalaPrefix));
}

}
