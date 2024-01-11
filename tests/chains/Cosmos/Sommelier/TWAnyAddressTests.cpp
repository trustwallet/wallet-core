// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gSommelierAddr = "somm1mry47pkga5tdswtluy0m8teslpalkdq0jalzdl";
static const std::string gSommelierHrp = "somm";

TEST(TWSommelierAnyAddress, AllSommelierAddressTests) {
    CosmosAddressParameters parameters{.hrp = gSommelierHrp, .coinType = TWCoinTypeSommelier, .address = gSommelierAddr};
    TestCosmosAddressParameters(parameters);
}

}
