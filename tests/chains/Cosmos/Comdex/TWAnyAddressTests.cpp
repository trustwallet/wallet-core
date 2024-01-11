// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gComdexAddr = "comdex1mry47pkga5tdswtluy0m8teslpalkdq0ewjv9z";
static const std::string gComdexHrp = "comdex";

TEST(TWComdexAnyAddress, AllComdexAddressTests) {
    CosmosAddressParameters parameters{.hrp = gComdexHrp, .coinType = TWCoinTypeComdex, .address = gComdexAddr};
    TestCosmosAddressParameters(parameters);
}

}
