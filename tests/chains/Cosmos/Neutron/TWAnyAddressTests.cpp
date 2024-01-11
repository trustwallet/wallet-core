// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gNeutronAddr = "neutron1mry47pkga5tdswtluy0m8teslpalkdq067evxj";
static const std::string gNeutronHrp = "neutron";

TEST(TWNeutronAnyAddress, AllNeutronAddressTests) {
    CosmosAddressParameters parameters{.hrp = gNeutronHrp, .coinType = TWCoinTypeNeutron, .address = gNeutronAddr};
    TestCosmosAddressParameters(parameters);
}

}
