// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gCrescentAddr = "cre1mry47pkga5tdswtluy0m8teslpalkdq06frtfc";
static const std::string gCrescentHrp = "cre";

TEST(TWCrescentAnyAddress, AllCrescentAddressTests) {
    CosmosAddressParameters parameters{.hrp = gCrescentHrp, .coinType = TWCoinTypeCrescent, .address = gCrescentAddr};
    TestCosmosAddressParameters(parameters);
}

}
