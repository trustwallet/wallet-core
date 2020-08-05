// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Groestlcoin/Address.h"

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Groestlcoin;

TEST(GroestlcoinAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("03b85cc59b67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91"), TWPublicKeyTypeSECP256k1);
    const auto address = Address(publicKey, 36);
    ASSERT_EQ(address.string(), "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM");
}

TEST(GroestlcoinAddress, Valid) {
    ASSERT_TRUE(Address::isValid(std::string("Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM")));
}

TEST(GroestlcoinAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(std::string("1JAd7XCBzGudGpJQSDSfpmJhiygtLQWaGL"))); // Valid bitcoin address
}

TEST(GroestlcoinAddress, FromString) {
    const auto string = "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM";
    const auto address = Address(string);

    ASSERT_EQ(address.string(), string);
}

TEST(GroestlcoinAddress, Derive) {
    const auto mnemonic = "all all all all all all all all all all all all";
    const auto wallet = HDWallet(mnemonic, "");
    const auto path = TW::derivationPath(TWCoinTypeGroestlcoin);
    const auto address = TW::deriveAddress(TWCoinTypeGroestlcoin, wallet.getKey(TWCoinTypeGroestlcoin, path));
    ASSERT_EQ(address, "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne");
}
