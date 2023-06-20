// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>

#include "Coin.h"
#include "HexCoding.h"
#include "HDWallet.h"
#include "Bitcoin/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Bitcoin;

TEST(BitcoinDiamondAddress, Valid) {
    ASSERT_TRUE(Address::isValid(std::string("1G15VvshDxwFTnahZZECJfFwEkq9fP79o8")));
    ASSERT_TRUE(Address::isValid(std::string("3PaCtJcD7sxpCf3JNPLpMNnHnwJHoBeucF")));
    ASSERT_TRUE(Address::isValid(std::string("395mRH5aV3DLHPXC4Md9cPdpiquLtTqRSX")));
}

TEST(BitcoinDiamondAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("02485a209514cc896f8ed736e205bc4c35bd5299ef3f9e84054475336b964c02a3"), TWPublicKeyTypeSECP256k1);
    const auto address = Address(publicKey, TWCoinTypeP2pkhPrefix(TWCoinTypeBitcoinDiamond));
    EXPECT_EQ(address.string(), "1G15VvshDxwFTnahZZECJfFwEkq9fP79o8");

    auto wallet = HDWallet("shoot island position soft burden budget tooth cruel issue economy destroy above", "");
    auto addr = wallet.deriveAddress(TWCoinTypeBitcoinDiamond);
    EXPECT_EQ(addr, "1KaRW9xPPtCTZ9FdaTHduCPck4YvSeEWNn");

    addr = wallet.deriveAddress(TWCoinTypeBitcoinDiamond, TWDerivationBitcoinSegwit);
    EXPECT_EQ(addr, "bcd1q7jh5qukuy9fc2pjm89xnyvx5dtfyvru9evw30x");
}

TEST(BitcoinDiamondAddress, FromString) {
    auto address = Address("1G15VvshDxwFTnahZZECJfFwEkq9fP79o8");
    ASSERT_EQ(address.string(), "1G15VvshDxwFTnahZZECJfFwEkq9fP79o8");

    address = Address("3PaCtJcD7sxpCf3JNPLpMNnHnwJHoBeucF");
    ASSERT_EQ(address.string(), "3PaCtJcD7sxpCf3JNPLpMNnHnwJHoBeucF");
}

TEST(BitcoinDiamondAddress, AddressData) {
    const auto publicKey = PublicKey(parse_hex("02485a209514cc896f8ed736e205bc4c35bd5299ef3f9e84054475336b964c02a3"), TWPublicKeyTypeSECP256k1);
    auto address = TW::deriveAddress(TWCoinTypeBitcoinDiamond, publicKey, TWDerivationBitcoinSegwit);

    auto data = TW::addressToData(TWCoinTypeBitcoinDiamond, "1G15VvshDxwFTnahZZECJfFwEkq9fP79o8");
    ASSERT_EQ(hex(data), "a48da46386ce52cccad178de900c71f06130c310");
    ASSERT_EQ(data, TW::addressToData(TWCoinTypeBitcoinDiamond, address));

    data = TW::addressToData(TWCoinTypeBitcoinDiamond, "1G15VvshDxwFTnahZZECJfFwEkq9fP79"); // invalid address
    ASSERT_EQ(data.size(), 0ul);
}
