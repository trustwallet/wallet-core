// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include "../interface/TWTestUtilities.h"
#include "Bitcoin/Address.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"
#include "Coin.h"
#include <gtest/gtest.h>
#include <TrustWalletCore/TWAnyAddress.h>

using namespace TW;

const char *AYA_PRIVATE_KEY = "403769C2CC212A04B13830EA108DCABF8EC87DDD1DCBB1E9BB8F6E539649DE8E";
const char *AYA_ADDRESS = "AWWMxHsQUi58cSePL4EbBcodbUMZ1H1KNf";
const char *AYA_FAKEADDRESS =  "AVsR9tT1XjrgrU1S2MaisATakwdkJvkck";

TEST(TWAryacoinAddress, Valid) {
    ASSERT_TRUE(Bitcoin::Address::isValid(std::string(AYA_ADDRESS)));
    ASSERT_FALSE(Bitcoin::Address::isValid(std::string(AYA_FAKEADDRESS)));
}

TEST(TWAryacoinAddress, PubkeyToAddress) {
    const auto privateKey = PrivateKey(parse_hex(AYA_PRIVATE_KEY));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);

    /// construct with public key
    auto address = Bitcoin::Address(PublicKey(publicKey), p2pkhPrefix(TWCoinTypeAryacoin));
    ASSERT_EQ(address.string(), AYA_ADDRESS);
}
