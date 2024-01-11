// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Zcash/TAddress.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

namespace TW::Zcash {

TEST(ZcashAddress, FromPrivateKey) {
    const auto privateKey =  PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto address = TAddress(publicKey);

    EXPECT_EQ(address.string(), "t1Wg9uPPAfwhBWeRjtDPa5ZHNzyBx9rJVKY");
    EXPECT_EQ(address.bytes[0], 0x1c);
    EXPECT_EQ(address.bytes[1], 0xb8);
}

TEST(ZcashAddress, FromPublicKey) {
    const auto privateKey = PrivateKey(parse_hex("BE88DF1D0BF30A923CB39C3BB953178BAAF3726E8D3CE81E7C8462E046E0D835"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto address = TAddress(publicKey);

    EXPECT_EQ(address.string(), "t1gaySCXCYtXE3ygP38YuWtVZczsEbdjG49");
    EXPECT_EQ(address.bytes[0], 0x1c);
    EXPECT_EQ(address.bytes[1], 0xb8);
}

TEST(ZcashAddress, Valid) {
    EXPECT_TRUE(TAddress::isValid(std::string("t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBy")));
    EXPECT_TRUE(TAddress::isValid(std::string("t1TWk2mmvESDnE4dmCfT7MQ97ij6ZqLpNVU")));
    EXPECT_TRUE(TAddress::isValid(std::string("t3RD6RFKhWSotNbPEY4Vw7Ku9QCfKkzrbBL")));
}

TEST(ZcashAddress, Invalid) {
    EXPECT_FALSE(TAddress::isValid(std::string("abc")));
    EXPECT_FALSE(TAddress::isValid(std::string("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed")));
    EXPECT_FALSE(TAddress::isValid(std::string("175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W")));
    EXPECT_FALSE(TAddress::isValid(std::string("t1RygJmrLdNGgi98+UgEJDTVaELTAYWoMBy"))); // Invalid Base58
    EXPECT_FALSE(TAddress::isValid(std::string("t1RygJmrLdNGgi98gUgEJDTVaELTAYW"))); // too short
    EXPECT_FALSE(TAddress::isValid(std::string("t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBz"))); // bad checksum
    EXPECT_FALSE(TAddress::isValid(std::string("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC"))); // too short
    EXPECT_FALSE(TAddress::isValid(std::string("2NRbuP5YfzRNEa1RibT5kXay1VgvQHnydZY1"))); // invalid prefix
}

TEST(ZcashAddress, InitWithString) {
    {
        const auto address = TAddress("t1Wg9uPPAfwhBWeRjtDPa5ZHNzyBx9rJVKY");
        EXPECT_EQ(address.string(), "t1Wg9uPPAfwhBWeRjtDPa5ZHNzyBx9rJVKY");
        EXPECT_EQ(address.bytes[0], 0x1c);
        EXPECT_EQ(address.bytes[1], 0xb8);
    }
    {
        const auto address = TAddress("t3RD6RFKhWSotNbPEY4Vw7Ku9QCfKkzrbBL");
        EXPECT_EQ(address.string(), "t3RD6RFKhWSotNbPEY4Vw7Ku9QCfKkzrbBL");
        EXPECT_EQ(address.bytes[0], 0x1c);
        EXPECT_EQ(address.bytes[1], 0xbd);
    }
}

} // namespace TW::Zcash
