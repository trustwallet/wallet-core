// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gCoreumAddr = "core1a5nvz6smgsph9gephguyhn30fmzrpaxrvvdjun";
static const std::string gCoreumHrp = "core";

TEST(TWCoreumAnyAddress, AllCoreumAddressTests) {
    CosmosAddressParameters parameters{.hrp = gCoreumHrp,
                                       .coinType = TWCoinTypeCoreum,
                                       .address = gCoreumAddr,
                                       .privKey = "56e5e45bf33a779527ec670b5336f6bc78efbe0e3bf1f004e7250673a82a3431",
                                       .publicKey = "0345d8d927b955c3cd468d12b5bc634c7919ee4777e578439af6314cf04b2ff114"};
    TestCosmosAddressParameters(parameters);
}

}
