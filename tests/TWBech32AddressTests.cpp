// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "gtest/gtest.h"
#include "TWTestUtilities.h"

#include <TrustWalletCore/TWBech32Address.h>
#include <TrustWalletCore/TWPublicKey.h>

const char *address1 = "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4";
const char *address2 = "bc1qr583w2swedy2acd7rung055k8t3n7udp7vyzyg";

TEST(TWBech32Address, PublicKeyToAddress) {
    auto pkData = DATA("0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
    auto publicKey = TWPublicKey();
    TWPublicKeyInitWithData(&publicKey, pkData.get());
    
    auto address = WRAP(TWBech32Address, TWBech32AddressCreateWithPublicKey(TWHRPBitcoin, publicKey));
    auto string = WRAPS(TWBech32AddressDescription(address.get()));

    ASSERT_STREQ(address1, TWStringUTF8Bytes(string.get()));
}

TEST(TWBech32Address, InitWithAddress) {
    auto string = STRING(address1);
    auto address = WRAP(TWBech32Address, TWBech32AddressCreateWithString(string.get()));
    auto description = WRAPS(TWBech32AddressDescription(address.get()));

    ASSERT_TRUE(address.get() != nullptr);
    ASSERT_STREQ(address1, TWStringUTF8Bytes(description.get()));
}

TEST(TWBech32Address, InvalidAddress) {
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
        ASSERT_TRUE(TWBech32AddressCreateWithString(string.get()) == nullptr) << "Invalid address '" << TWStringUTF8Bytes(string.get()) << "' reported as valid.";
    }
}
