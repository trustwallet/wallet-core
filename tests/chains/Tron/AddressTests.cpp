// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Tron/Address.h"

#include <gtest/gtest.h>

namespace TW::Tron {

TEST(TronAddress, FromPublicKey) {
    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    const auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");

    const auto privateKey2 = PrivateKey(parse_hex("BE88DF1D0BF30A923CB39C3BB953178BAAF3726E8D3CE81E7C8462E046E0D835"));
    const auto publicKey2 = privateKey2.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    const auto address2 = Address(publicKey2);
    ASSERT_EQ(address2.string(), "THRF3GuPnvvPzKoaT8pJex5XHmo8NNbCb3");

    const auto privateKey3 = PrivateKey(parse_hex("BE88DF1D0BF30A923CB39C3BB953178BAAF3726E8D3CE81E7C8462E046E0D835"));
    const auto publicKey3 = privateKey3.getPublicKey(TWPublicKeyTypeED25519);
    EXPECT_ANY_THROW(new Address(publicKey3));
}

TEST(TronAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(std::string("abc")));
    ASSERT_FALSE(Address::isValid(std::string("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")));
    ASSERT_FALSE(Address::isValid(std::string("175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W")));
    ASSERT_FALSE(Address::isValid(std::string("2MegQ6oqSda2tTagdEzBA")));
    ASSERT_TRUE(Address::isValid(std::string("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC")));
}

TEST(TronAddress, InitWithString) {
    const auto address = Address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");

    ASSERT_EQ(address.string(), "TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
}

} // namespace TW::Tron
