// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gTiaAddr = "celestia1mry47pkga5tdswtluy0m8teslpalkdq00tp7xc";
static const std::string gTiaHrp = "celestia";

TEST(TWTiaAnyAddress, AllTiaAddressTests) {
    CosmosAddressParameters parameters{.hrp = gTiaHrp, .coinType = TWCoinTypeTia, .address = gTiaAddr};
    TestCosmosAddressParameters(parameters);
}

}
 