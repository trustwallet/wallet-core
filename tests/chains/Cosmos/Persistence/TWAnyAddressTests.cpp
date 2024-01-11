// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gPersistenceAddr = "persistence1mry47pkga5tdswtluy0m8teslpalkdq0sdkaj3";
static const std::string gPersistenceHrp = "persistence";

TEST(TWPersistenceAnyAddress, AllPersistenceAddressTests) {
    CosmosAddressParameters parameters{.hrp = gPersistenceHrp, .coinType = TWCoinTypePersistence, .address = gPersistenceAddr};
    TestCosmosAddressParameters(parameters);
}

}
