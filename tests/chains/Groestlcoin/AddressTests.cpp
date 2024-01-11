// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Groestlcoin/Address.h"

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Groestlcoin::tests {

TEST(GroestlcoinAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("03b85cc59b67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91"), TWPublicKeyTypeSECP256k1);
    const auto address = Address(publicKey, 36);
    ASSERT_EQ(address.string(), "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM");

    const auto privateKey = PrivateKey(parse_hex("a1269039e4ffdf43687852d7247a295f0b5bc55e6dda031cffaa3295ca0a9d7a"));
    const auto publicKey2 = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    EXPECT_ANY_THROW(new Address(publicKey2, 36));
}

TEST(GroestlcoinAddress, Valid) {
    ASSERT_TRUE(Address::isValid(std::string("Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM")));
    TW::Data addressData;
    addressData.push_back(TW::p2pkhPrefix(TWCoinTypeGroestlcoin));

    auto addressHash = parse_hex("98af0aaca388a7e1024f505c033626d908e3b54a");
    std::copy(addressHash.begin(), addressHash.end(), std::back_inserter(addressData));

    ASSERT_EQ(Address(addressData).string(), "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM");
}

TEST(GroestlcoinAddress, Invalid) {
    EXPECT_EXCEPTION(Address("Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLo"), "Invalid address string");                    // Invalid address
    EXPECT_EXCEPTION(Address(parse_hex("98af0aaca388a7e1024f505c033626d908e3b5")), "Invalid address key data"); // Invalid address data
    ASSERT_FALSE(Address::isValid(std::string("1JAd7XCBzGudGpJQSDSfpmJhiygtLQWaGL")));                          // Valid bitcoin address
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
    auto address = TW::deriveAddress(TWCoinTypeGroestlcoin, wallet.getKey(TWCoinTypeGroestlcoin, path));
    ASSERT_EQ(address, "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne");

    address = TW::deriveAddress(TWCoinTypeGroestlcoin, wallet.getKey(TWCoinTypeGroestlcoin, path), TWDerivationBitcoinLegacy);
    ASSERT_EQ(address, "FfsAQrzfdECwEsApubn2rvxgamU8CcqsLT");
}

TEST(GroestlcoinAddress, AddressData) {
    const auto publicKey = PublicKey(parse_hex("03b85cc59b67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91"), TWPublicKeyTypeSECP256k1);
    auto address = TW::deriveAddress(TWCoinTypeGroestlcoin, publicKey, TWDerivationBitcoinLegacy);
    ASSERT_EQ(address, "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM");

    auto addressData = TW::addressToData(TWCoinTypeGroestlcoin, address);
    ASSERT_EQ(hex(addressData), "98af0aaca388a7e1024f505c033626d908e3b54a");

    address = TW::deriveAddress(TWCoinTypeGroestlcoin, publicKey, TWDerivationBitcoinSegwit);
    EXPECT_EQ(address, "grs1qnzhs4t9r3zn7zqj02pwqxd3xmyyw8d22q55nf8");

    addressData = TW::addressToData(TWCoinTypeGroestlcoin, address);
    EXPECT_EQ(hex(addressData), "98af0aaca388a7e1024f505c033626d908e3b54a");
}

} // namespace TW::Groestlcoin::tests
