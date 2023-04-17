// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CosmosTestHelpers.h"
#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Cosmos::tests {

static const std::string gStrideAddr = "stride1mry47pkga5tdswtluy0m8teslpalkdq0a2sjge";
static const std::string gStrideHrp = "stride";

TEST(TWStrideAnyAddress, AllStrideAddressTests) {
    CosmosAddressParameters parameters{.hrp = gStrideHrp, .coinType = TWCoinTypeStride, .address = gStrideAddr};
    TestCosmosAddressParameters(parameters);
}

}
