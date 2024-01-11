// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gKujiraAddr = "kujira1mry47pkga5tdswtluy0m8teslpalkdq00fjk3l";
static const std::string gKujiraHrp = "kujira";

TEST(TWKujiraAnyAddress, AllKujiraAddressTests) {
    CosmosAddressParameters parameters{.hrp = gKujiraHrp, .coinType = TWCoinTypeKujira, .address = gKujiraAddr};
    TestCosmosAddressParameters(parameters);
}

}
