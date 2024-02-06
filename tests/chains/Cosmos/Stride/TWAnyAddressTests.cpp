// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"
#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Cosmos::tests {

static const std::string gStrideAddr = "stride1mry47pkga5tdswtluy0m8teslpalkdq0a2sjge";
static const std::string gStrideHrp = "stride";

TEST(TWStrideAnyAddress, AllStrideAddressTests) {
    CosmosAddressParameters parameters{.hrp = gStrideHrp, .coinType = TWCoinTypeStride, .address = gStrideAddr};
    TestCosmosAddressParameters(parameters);
}

}
