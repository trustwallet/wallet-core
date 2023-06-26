// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gAkashAddr = "akash1mry47pkga5tdswtluy0m8teslpalkdq0n6af90";
static const std::string gAkashHrp = "akash";

TEST(TWAkashAnyAddress, AllAkashAddressTests) {
    CosmosAddressParameters parameters{.hrp = gAkashHrp, .coinType = TWCoinTypeAkash, .address = gAkashAddr};
    TestCosmosAddressParameters(parameters);
}

}
