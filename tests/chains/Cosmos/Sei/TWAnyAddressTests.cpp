// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gSeiAddr = "sei1mry47pkga5tdswtluy0m8teslpalkdq0ndpc65";
static const std::string gSeiHrp = "sei";

TEST(TWSeiAnyAddress, AllSeiAddressTests) {
    CosmosAddressParameters parameters{.hrp = gSeiHrp, .coinType = TWCoinTypeSei, .address = gSeiAddr};
    TestCosmosAddressParameters(parameters);
}

}
