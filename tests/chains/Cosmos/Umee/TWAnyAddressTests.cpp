// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gUmeeAddr = "umee1mry47pkga5tdswtluy0m8teslpalkdq0vhd3c8";
static const std::string gUmeeHrp = "umee";

TEST(TWUmeeAnyAddress, AllUmeeAddressTests) {
    CosmosAddressParameters parameters{.hrp = gUmeeHrp, .coinType = TWCoinTypeUmee, .address = gUmeeAddr};
    TestCosmosAddressParameters(parameters);
}

}
