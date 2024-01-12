// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Waves/Address.h"

#include <Coin.h>
#include <HDWallet.h>
#include <gtest/gtest.h>

using namespace std;
using namespace TW;

namespace TW::Waves::tests {

TEST(WavesAddress, SecureHash) {
    const auto secureHash =
        hex(Address::secureHash(parse_hex("0157c7fefc0c6acc54e9e4354a81ac1f038e01745731")));

    ASSERT_EQ(secureHash, "a7978a753c6496866dc75ba3abcaaec796f2380037a1fa7c46cbf9762ee380df");
}

TEST(WavesAddress, FromPrivateKey) {
    const auto privateKey =
        PrivateKey(parse_hex("9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a"));
    const auto publicKeyEd25519 = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    ASSERT_EQ(hex(Data(publicKeyEd25519.bytes.begin(), publicKeyEd25519.bytes.end())),
              "ff84c4bfc095df25b01e48807715856d95af93d88c5b57f30cb0ce567ca4ced6");
    const auto publicKeyCurve25519 = privateKey.getPublicKey(TWPublicKeyTypeCURVE25519);
    ASSERT_EQ(hex(Data(publicKeyCurve25519.bytes.begin(), publicKeyCurve25519.bytes.end())),
              "559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d");
    const auto address = Address(publicKeyCurve25519);

    ASSERT_EQ(address.string(), "3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds");

    const auto publicKeySECP256k1 = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    EXPECT_ANY_THROW(new Address(publicKeySECP256k1));
}

TEST(WavesAddress, FromPublicKey) {
    const auto publicKey =
        PublicKey(parse_hex("559a50cb45a9a8e8d4f83295c354725990164d10bb505275d1a3086c08fb935d"),
                  TWPublicKeyTypeCURVE25519);
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds");

    const auto address2 = Address(Data(address.bytes.begin(), address.bytes.end()));
    ASSERT_EQ(address2.string(), "3P2uzAzX9XTu1t32GkWw68YFFLwtapWvDds");
}

TEST(WavesAddress, FromData) {
    EXPECT_ANY_THROW(new Address(Data{}));
}

TEST(WavesAddress, Invalid) {
    ASSERT_FALSE(Address::isValid(std::string("abc")));
    ASSERT_FALSE(Address::isValid(std::string("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")));
    ASSERT_FALSE(Address::isValid(std::string("3PLANf4MgtNN5v5k4NNnyx2m4zKJiw1tF9v")));
    ASSERT_FALSE(Address::isValid(std::string("3PLANf4MgtNN5v6k4NNnyx2m4zKJiw1tF8v")));
}

TEST(WavesAddress, Valid) {
    ASSERT_TRUE(Address::isValid(std::string("3PLANf4MgtNN5v6k4NNnyx2m4zKJiw1tF9v")));
    ASSERT_TRUE(Address::isValid(std::string("3PDjjLFDR5aWkKgufika7KSLnGmAe8ueDpC")));
    ASSERT_TRUE(Address::isValid(std::string("3PLjucTjqEfmgBF7fs2CER3fHQapCtknPeW")));
    ASSERT_TRUE(Address::isValid(std::string("3PB9ffP1YKQer3e7t283gPCLyjEfK8xrGp7")));
}

TEST(WavesAddress, InitWithString) {
    const auto address = Address("3PQupTC1yRiHneotFt79LF2pkN6GrGMwEy3");
    ASSERT_EQ(address.string(), "3PQupTC1yRiHneotFt79LF2pkN6GrGMwEy3");
}

TEST(WavesAddress, InitWithInvalidString) {
    EXPECT_THROW(Address("3PQupTC1yRiHneotFt79LF2pkN6GrGMwEy2"), invalid_argument);
}

TEST(WavesAddress, Derive) {
    const auto mnemonic =
        "water process satisfy repeat flag avoid town badge sketch surge split between cabin sugar "
        "ill special axis adjust pull useful craft peace flee physical";
    const auto wallet = HDWallet(mnemonic, "");
    const auto address1 = TW::deriveAddress(
        TWCoinTypeWaves, wallet.getKey(TWCoinTypeWaves, DerivationPath("m/44'/5741564'/0'/0'/0'")));
    const auto address2 = TW::deriveAddress(
        TWCoinTypeWaves, wallet.getKey(TWCoinTypeWaves, DerivationPath("m/44'/5741564'/0'/0'/1'")));

    ASSERT_EQ(address1, "3PQupTC1yRiHneotFt79LF2pkN6GrGMwEy3");
    ASSERT_EQ(address2, "3PEXw52bkS9XuLhttWoKyykZjXqEY8zeLxf");
}

} // namespace TW::Waves::tests
