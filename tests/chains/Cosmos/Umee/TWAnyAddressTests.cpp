// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gUmeeAddr = "umee1mry47pkga5tdswtluy0m8teslpalkdq0vhd3c8";
static const std::string gUmeeHrp = "umee";

TEST(TWUmeeAnyAddress, AllUmeeAddressTests) {
    CosmosAddressParameters parameters{.hrp = gUmeeHrp, .coinType = TWCoinTypeUmee, .address = gUmeeAddr};
    TestCosmosAddressParameters(parameters);
}

}
