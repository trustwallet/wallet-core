// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gJunoAddr = "juno1mry47pkga5tdswtluy0m8teslpalkdq0gnn4mf";
static const std::string gJunoHrp = "juno";

TEST(TWJunoAnyAddress, AllJunoAddressTests) {
    CosmosAddressParameters parameters{.hrp = gJunoHrp, .coinType = TWCoinTypeJuno, .address = gJunoAddr};
    TestCosmosAddressParameters(parameters);
}

}
