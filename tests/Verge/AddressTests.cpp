// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Bitcoin/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

#include <TrustWalletCore/TWCoinType.h>

using namespace TW;
using namespace TW::Bitcoin;

TEST(VergeAddress, Valid) {
    // verge(xvg) only support legacy address
    ASSERT_TRUE(Address::isValid(std::string("DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E")));
}

TEST(VergeAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(std::string("DRyNFvJaybnF22UfMS6NR1Qav3mqxPj234")));
}

TEST(VergeAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("693dfe6f3ed717573eb10c24ebe5eb592fa3c239245cd499c487eb7b8ea7ed3a"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1), TWCoinTypeP2pkhPrefix(TWCoinTypeVerge));
    ASSERT_EQ(address.string(), "DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E");
}

TEST(VergeAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("034f3eb727ca1eba84a0d22839a483a1120ee6a1da0d5087dde527b5ff912c1694"), TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey, TWCoinTypeP2pkhPrefix(TWCoinTypeVerge));
    ASSERT_EQ(address.string(), "D8rBdwBfz5wvLhmHvRkXnNzeeihQgxkLmL");
}

TEST(VergeAddress, FromString) {
    auto address = Address("DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E");
    ASSERT_EQ(address.string(), "DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E");
}
