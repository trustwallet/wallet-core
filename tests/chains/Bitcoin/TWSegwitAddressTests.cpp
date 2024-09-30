// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include "Data.h"
#include "HexCoding.h"
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWSegwitAddress.h>

#include <gtest/gtest.h>

const char* address1 = "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4";
const char* address2 = "bc1qr583w2swedy2acd7rung055k8t3n7udp7vyzyg";
const char* address3Taproot = "bc1ptmsk7c2yut2xah4pgflpygh2s7fh0cpfkrza9cjj29awapv53mrslgd5cf";

TEST(TWSegwitAddress, CreateAndDelete) {
    {
        TWSegwitAddress* addr = TWSegwitAddressCreateWithString(STRING(address1).get());
        EXPECT_TRUE(addr != nullptr);
        TWSegwitAddressDelete(addr);
    }
    {
        auto pkData = DATA("0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
        auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(pkData.get(), TWPublicKeyTypeSECP256k1));
        TWSegwitAddress* addr = TWSegwitAddressCreateWithPublicKey(TWHRPBitcoin, publicKey.get());
        EXPECT_TRUE(addr != nullptr);
        TWSegwitAddressDelete(addr);
    }
}

TEST(TWSegwitAddress, PublicKeyToAddress) {
    auto pkData = DATA("0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(pkData.get(), TWPublicKeyTypeSECP256k1));

    auto address = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithPublicKey(TWHRPBitcoin, publicKey.get()));
    auto string = WRAPS(TWSegwitAddressDescription(address.get()));

    ASSERT_STREQ(address1, TWStringUTF8Bytes(string.get()));

    auto str = STRING(address1);
    auto addr = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithString(string.get()));
    ASSERT_TRUE(TWSegwitAddressEqual(address.get(), addr.get()));
}

TEST(TWSegwitAddress, InitWithAddress) {
    auto string = STRING(address1);
    ASSERT_TRUE(TWSegwitAddressIsValidString(string.get()));

    auto address = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithString(string.get()));
    auto description = WRAPS(TWSegwitAddressDescription(address.get()));

    ASSERT_TRUE(address.get() != nullptr);
    ASSERT_STREQ(address1, TWStringUTF8Bytes(description.get()));

    ASSERT_EQ(0, TWSegwitAddressWitnessVersion(address.get()));

    ASSERT_EQ(TWHRPBitcoin, TWSegwitAddressHRP(address.get()));

    auto witness = WRAPD(TWSegwitAddressWitnessProgram(address.get()));
    ASSERT_EQ(TW::hex(TW::data(TWDataBytes(witness.get()), TWDataSize(witness.get()))), "751e76e8199196d454941c45d1b3a323f1433bd6");
}

TEST(TWSegwitAddress, TaprootString) {
    const auto string = STRING(address3Taproot);
    const auto address = WRAP(TWSegwitAddress, TWSegwitAddressCreateWithString(string.get()));
    ASSERT_TRUE(address.get() != nullptr);

    const auto description = WRAPS(TWSegwitAddressDescription(address.get()));
    ASSERT_STREQ(address3Taproot, TWStringUTF8Bytes(description.get()));

    ASSERT_EQ(1, TWSegwitAddressWitnessVersion(address.get())); // taproot has segwit version 1

    ASSERT_EQ(TWHRPBitcoin, TWSegwitAddressHRP(address.get()));
}

TEST(TWSegwitAddress, InvalidAddress) {
    std::vector<std::shared_ptr<TWString>> strings = {
        STRING("bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t5"),
        STRING("bc1rw5uspcuh"),
        STRING("bc10w508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kw5rljs90"),
        STRING("BC1QR508D6QEJXTDG4Y5R3ZARVARYV98GJ9P"),
        STRING("tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sL5k7"),
        STRING("bc1zw508d6qejxtdg4y5r3zarvaryvqyzf3du"),
        STRING("tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3pjxtptv"),
        STRING("bc1gmk9yu"),
    };
    for (auto& string : strings) {
        ASSERT_TRUE(TWSegwitAddressCreateWithString(string.get()) == nullptr) << "Invalid address '" << TWStringUTF8Bytes(string.get()) << "' reported as valid.";
    }
}
