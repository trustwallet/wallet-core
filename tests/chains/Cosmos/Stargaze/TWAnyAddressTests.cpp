// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWCoinType.h>
#include "TestUtilities.h"
#include <gtest/gtest.h>
#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gStarsAddr = "stars1mry47pkga5tdswtluy0m8teslpalkdq02a8nhy";
static const std::string gStarsHrp = "stars";

TEST(TWStargazeAnyAddress, AllStargazeAddressTests) {
    CosmosAddressParameters parameters{.hrp = gStarsHrp, .coinType = TWCoinTypeStargaze, .address = gStarsAddr};
    TestCosmosAddressParameters(parameters);
}

}
