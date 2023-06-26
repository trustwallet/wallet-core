// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CosmosTestHelpers.h"

namespace TW::Cosmos::tests {

static const std::string gNativeCantoAddr = "canto14py36sx57ud82t9yrks9z6hdsrpn5x6kl5l2ap";
static const std::string gNativeCantoHrp = "canto";

TEST(TWNativeCantoAnyAddress, AllNativeCantoAddressTests) {
    CosmosAddressParameters parameters{.hrp = gNativeCantoHrp,
                                       .coinType = TWCoinTypeNativeCanto,
                                       .address = gNativeCantoAddr,
                                       .standaloneChain = false,
                                       .publicKeyType = TWPublicKeyTypeSECP256k1Extended,
                                       .privKey = "8d2a3bd62d300a148c89dc8635f87b7a24a951bd1c4e78675fe40e1a640d46ed",
                                       .publicKey = "04868e7e1634417db2adfd9fe38205bfa0fea01898a7fd30565d13f7056a37c065211845f6e553524c2c1611af9712ac02b7a3b439c9f0cfcadfd81a2c86cc0ab8",
                                        };
    TestCosmosAddressParameters(parameters);
}

} // namespace TW::Cosmos::tests
