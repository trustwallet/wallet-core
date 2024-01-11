// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gMarsAddr = "mars1mry47pkga5tdswtluy0m8teslpalkdq0rufhfw";
static const std::string gMarsHrp = "mars";

TEST(TWMarsAnyAddress, AllMarsAddressTests) {
    CosmosAddressParameters parameters{.hrp = gMarsHrp, .coinType = TWCoinTypeMars, .address = gMarsAddr};
    TestCosmosAddressParameters(parameters);
}

}
