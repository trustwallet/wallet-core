// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gAtomAddr = "cosmos1mry47pkga5tdswtluy0m8teslpalkdq07pswu4";
static const std::string gAtomHrp = "cosmos";

TEST(TWAtomAnyAddress, AllAtomAddressTests) {
    CosmosAddressParameters parameters{.hrp = gAtomHrp, .coinType = TWCoinTypeCosmos, .address = gAtomAddr};
    TestCosmosAddressParameters(parameters);
}

}
