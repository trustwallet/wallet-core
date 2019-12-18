// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Cosmos/Address.h"

#include <gtest/gtest.h>
#include <TrustWalletCore/TWHDWallet.h>

namespace TW::Cosmos {

TEST(Address, CoinexValid) {
    ASSERT_TRUE(Address::isValid("coinex1pxf3sgcxpu99e4havmeus7gmmt8u3j9j6ra6me"));
}

TEST(Address, CoinexInvalid) {
    ASSERT_FALSE(Address::isValid("coinex1pxf3sgcxpu99e4havmeus7gmmt8u3j9j6ra63"));
}

TEST(Address, CoinexFromKeyHash) {
    auto privateKey = PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"));
    auto publicKeyData = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ(hex(publicKeyData.bytes.begin(), publicKeyData.bytes.end()), "024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382");

    auto publicKey = PublicKey(publicKeyData);
    auto address = Address("coinex", publicKey);
    ASSERT_EQ(address.string(), "coinex1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0juwxvr");
    auto address_2 = Address("cettest", publicKey);
    ASSERT_EQ(address_2.string(), "cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j");
}

} // namespace TW::Cosmos
