// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gQuasarAddr = "quasar1mry47pkga5tdswtluy0m8teslpalkdq0sz2n3s";
static const std::string gQuasarHrp = "quasar";

TEST(TWQuasarAnyAddress, AllQuasarAddressTests) {
    CosmosAddressParameters parameters{.hrp = gQuasarHrp, .coinType = TWCoinTypeQuasar, .address = gQuasarAddr};
    TestCosmosAddressParameters(parameters);
}

}
