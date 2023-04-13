// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gNeutronAddr = "neutron1mry47pkga5tdswtluy0m8teslpalkdq067evxj";
static const std::string gNeutronHrp = "neutron";

TEST(TWNeutronAnyAddress, AllNeutronAddressTests) {
    CosmosAddressParameters parameters{.hrp = gNeutronHrp, .coinType = TWCoinTypeNeutron, .address = gNeutronAddr};
    TestCosmosAddressParameters(parameters);
}

}
