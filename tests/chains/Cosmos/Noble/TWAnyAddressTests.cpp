// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gNobleAddr = "noble1mry47pkga5tdswtluy0m8teslpalkdq0kz9xym";
static const std::string gNobleHrp = "noble";

TEST(TWNobleAnyAddress, AllNobleAddressTests) {
    CosmosAddressParameters parameters{.hrp = gNobleHrp, .coinType = TWCoinTypeNoble, .address = gNobleAddr};
    TestCosmosAddressParameters(parameters);
}

}
