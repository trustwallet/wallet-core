// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gSecretAddr = "secret1l0cjpuwu09hwu4wdds7pljn83346x2c90d8h0l";
static const std::string gSecretHrp = "secret";

TEST(TWSecretAnyAddress, AllSecretAddressTests) {
    CosmosAddressParameters parameters{.hrp = gSecretHrp,
                                       .coinType = TWCoinTypeSecret,
                                       .address = gSecretAddr,
                                       .privKey = "a054c9a67d81ada560ab6fda3310ebf5971e163ff2291ee736ca64b6a5af1ada",
                                       .publicKey = "03967d2c6263c2d74d9c2fac3a024e2892a94497b64edb294ffab4042851f00b90"};
    TestCosmosAddressParameters(parameters);
}

} // namespace TW::Cosmos::tests
