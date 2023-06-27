// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gCrescentAddr = "cre1mry47pkga5tdswtluy0m8teslpalkdq06frtfc";
static const std::string gCrescentHrp = "cre";

TEST(TWCrescentAnyAddress, AllCrescentAddressTests) {
    CosmosAddressParameters parameters{.hrp = gCrescentHrp, .coinType = TWCoinTypeCrescent, .address = gCrescentAddr};
    TestCosmosAddressParameters(parameters);
}

}
