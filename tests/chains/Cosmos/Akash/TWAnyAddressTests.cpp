// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gAkashAddr = "akash1mry47pkga5tdswtluy0m8teslpalkdq0n6af90";
static const std::string gAkashHrp = "akash";

TEST(TWAkashAnyAddress, AllAkashAddressTests) {
    CosmosAddressParameters parameters{.hrp = gAkashHrp, .coinType = TWCoinTypeAkash, .address = gAkashAddr};
    TestCosmosAddressParameters(parameters);
}

}
