// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gOsmosisAddr = "osmo1mry47pkga5tdswtluy0m8teslpalkdq0k6r728";
static const std::string gOsmosisHrp = "osmo";

TEST(TWOsmosisAnyAddress, AllOsmosisAddressTests) {
    CosmosAddressParameters parameters{.hrp = gOsmosisHrp, .coinType = TWCoinTypeOsmosis, .address = gOsmosisAddr};
    TestCosmosAddressParameters(parameters);
}

}
