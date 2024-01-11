// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gCryptoorgAddr = "cro1tcfsr7m7d6jk6fpyety373m8c39ea2f8dmp830";
static const std::string gCryptoorgHrp = "cro";

TEST(TWCryptoorgAnyAddress, AllCryptoorgAddressTests) {
    CosmosAddressParameters parameters{.hrp = gCryptoorgHrp,
                                       .coinType = TWCoinTypeCryptoOrg,
                                       .address = gCryptoorgAddr,
                                       .privKey = "5469c1a88e67d6d490e647ac8d82d54c4a17b8f00d272b3b30fac2253339aa28",
                                       .publicKey = "025824f188c340235910b15e5e35aea11cfc28eabfa7756da5585c08f74db437ef"};
    TestCosmosAddressParameters(parameters);
}

} // namespace TW::Cosmos::tests
