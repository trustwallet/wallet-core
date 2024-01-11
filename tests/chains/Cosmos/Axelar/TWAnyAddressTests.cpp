// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gAxelarAddr = "axelar1mry47pkga5tdswtluy0m8teslpalkdq060xxh5";
static const std::string gAxelarHrp = "axelar";

TEST(TWAxelarAnyAddress, AllAxelarAddressTests) {
    CosmosAddressParameters parameters{.hrp = gAxelarHrp, .coinType = TWCoinTypeAxelar, .address = gAxelarAddr};
    TestCosmosAddressParameters(parameters);
}

}
