// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gComdexAddr = "comdex1mry47pkga5tdswtluy0m8teslpalkdq0ewjv9z";
static const std::string gComdexHrp = "comdex";

TEST(TWComdexAnyAddress, AllComdexAddressTests) {
    CosmosAddressParameters parameters{.hrp = gComdexHrp, .coinType = TWCoinTypeComdex, .address = gComdexAddr};
    TestCosmosAddressParameters(parameters);
}

}
