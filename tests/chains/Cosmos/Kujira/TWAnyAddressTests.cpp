// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gKujiraAddr = "kujira1mry47pkga5tdswtluy0m8teslpalkdq00fjk3l";
static const std::string gKujiraHrp = "kujira";

TEST(TWKujiraAnyAddress, AllKujiraAddressTests) {
    CosmosAddressParameters parameters{.hrp = gKujiraHrp, .coinType = TWCoinTypeKujira, .address = gKujiraAddr};
    TestCosmosAddressParameters(parameters);
}

}
