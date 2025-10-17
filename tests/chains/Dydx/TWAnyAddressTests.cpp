// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../Cosmos/CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gDydxAddr = "dydx1adl7usw7z2dnysyn7wvrghu0u0q6gr7jqs4gtt";
static const std::string gDydxHrp = "dydx";

TEST(TWDydxAnyAddress, AllDydxAddressTests) {
    CosmosAddressParameters parameters{.hrp = gDydxHrp, .coinType = TWCoinTypeDydx, .address = gDydxAddr};
    TestCosmosAddressParameters(parameters);
}

TEST(TWDydxAnyAddress, GovernanceAuthorityAddress) {
    // Test the governance authority address used in our tests
    const std::string authorityAddr = "dydx10d07y265gmmuvt4z0w9aw880jnsr700jnmapky";
    
    auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(STRING(authorityAddr.c_str()).get(), TWCoinTypeDydx));
    auto addressString = WRAPS(TWAnyAddressDescription(address.get()));
    
    EXPECT_TRUE(TWAnyAddressIsValid(STRING(authorityAddr.c_str()).get(), TWCoinTypeDydx));
    assertStringsEqual(addressString, authorityAddr);
}

}
