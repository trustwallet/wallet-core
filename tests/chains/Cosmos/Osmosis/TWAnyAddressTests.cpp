// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gOsmosisAddr = "osmo1mry47pkga5tdswtluy0m8teslpalkdq0k6r728";
static const std::string gOsmosisHrp = "osmo";

TEST(TWOsmosisAnyAddress, AllOsmosisAddressTests) {
    CosmosAddressParameters parameters{.hrp = gOsmosisHrp, .coinType = TWCoinTypeOsmosis, .address = gOsmosisAddr};
    TestCosmosAddressParameters(parameters);
}

}
