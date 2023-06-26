// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ronin/Address.h"

#include <gtest/gtest.h>

namespace TW::Ronin::tests {

TEST(RoninAddress, Create) {
    EXPECT_ANY_THROW(new TW::Ronin::Address(""));
}

} // namespace TW::Ronin::tests
