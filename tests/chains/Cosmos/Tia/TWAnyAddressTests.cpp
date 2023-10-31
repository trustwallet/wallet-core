// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gTiaAddr = "celestia1mry47pkga5tdswtluy0m8teslpalkdq00tp7xc";
static const std::string gTiaHrp = "celestia";

TEST(TWTiaAnyAddress, AllTiaAddressTests) {
    CosmosAddressParameters parameters{.hrp = gTiaHrp, .coinType = TWCoinTypeTia, .address = gTiaAddr};
    TestCosmosAddressParameters(parameters);
}

}
 