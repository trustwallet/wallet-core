// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gFetchAIAddr = "fetch1mry47pkga5tdswtluy0m8teslpalkdq0due27z";
static const std::string gFetchAIHrp = "fetch";

TEST(TWFetchAIAnyAddress, AllFetchAIAddressTests) {
    CosmosAddressParameters parameters{.hrp = gFetchAIHrp, .coinType = TWCoinTypeFetchAI, .address = gFetchAIAddr};
    TestCosmosAddressParameters(parameters);
}

}
