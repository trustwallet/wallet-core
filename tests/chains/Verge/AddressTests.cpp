// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
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
    ASSERT_TRUE(Address::isValid(std::string("DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E")));
    ASSERT_TRUE(TW::validateAddress(TWCoinTypeVerge, "DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E"));
    ASSERT_TRUE(TW::validateAddress(TWCoinTypeVerge, "vg1qujpe553lzgyg95g7k0w6zwscuy0ae022h4q4zg"));
}

TEST(VergeAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(std::string("DRyNFvJaybnF22UfMS6NR1Qav3mqxPj234")));
}

TEST(VergeAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("693dfe6f3ed717573eb10c24ebe5eb592fa3c239245cd499c487eb7b8ea7ed3a"));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey, TWCoinTypeP2pkhPrefix(TWCoinTypeVerge));
    ASSERT_EQ(address.string(), "DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E");

    auto addr = TW::deriveAddress(TWCoinTypeVerge, publicKey, TWDerivationBitcoinSegwit);
    ASSERT_EQ(addr, "vg1qujpe553lzgyg95g7k0w6zwscuy0ae022h4q4zg");
}

TEST(VergeAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("034f3eb727ca1eba84a0d22839a483a1120ee6a1da0d5087dde527b5ff912c1694"), TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey, TWCoinTypeP2pkhPrefix(TWCoinTypeVerge));
    ASSERT_EQ(address.string(), "D8rBdwBfz5wvLhmHvRkXnNzeeihQgxkLmL");

    auto addr = TW::deriveAddress(TWCoinTypeVerge, publicKey, TWDerivationBitcoinLegacy);
    ASSERT_EQ(addr, "D8rBdwBfz5wvLhmHvRkXnNzeeihQgxkLmL");
}

TEST(VergeAddress, FromString) {
    auto address = Address("DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E");
    ASSERT_EQ(address.string(), "DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E");

    auto data = TW::addressToData(TWCoinTypeVerge, "DRyNFvJaybnF22UfMS6NR1Qav3mqxPj86E");
    EXPECT_EQ(hex(data), "e4839a523f120882d11eb3dda13a18e11fdcbd4a");

    data = TW::addressToData(TWCoinTypeVerge, "vg1qujpe553lzgyg95g7k0w6zwscuy0ae022h4q4zg");
    EXPECT_EQ(hex(data), "e4839a523f120882d11eb3dda13a18e11fdcbd4a");

    // invalid address
    data = TW::addressToData(TWCoinTypeVerge, "vg1qujpe553lzgyg95g7k0w6zwscuy0ae022h4q4");
    EXPECT_EQ(data.size(), 0ul);
}
