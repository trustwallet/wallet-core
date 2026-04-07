#include "Zclassic/TAddress.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>

namespace TW::Zclassic {

TEST(ZclassicAddress, FromPrivateKey) {
    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"), TWCurveSECP256k1);
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto address = TAddress(publicKey);
    EXPECT_EQ(address.string(), "t1Wg9uPPAfwhBWeRjtDPa5ZHNzyBx9rJVKY");
    EXPECT_EQ(address.bytes[0], 0x1c);
    EXPECT_EQ(address.bytes[1], 0xb8);
}

TEST(ZclassicAddress, Valid) {
    EXPECT_TRUE(TAddress::isValid("t1TWk2mmvESDnE4dmCfT7MQ97ij6ZqLpNVU"));
    EXPECT_TRUE(TAddress::isValid("t3RD6RFKhWSotNbPEY4Vw7Ku9QCfKkzrbBL"));
}

TEST(ZclassicAddress, Invalid) {
    EXPECT_FALSE(TAddress::isValid("abc"));
    EXPECT_FALSE(TAddress::isValid("t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBz")); // bad checksum
}

} // namespace TW::Zclassic
