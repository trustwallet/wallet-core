// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Ethereum/EIP2645.h"
#include "HexCoding.h"
#include "ImmutableX/Constants.h"
#include "ImmutableX/StarkKey.h"
#include <gtest/gtest.h>

namespace TW::ImmutableX::tests {

TEST(ImmutableX, PathFromAddress) {
    // https://github.com/immutable/imx-core-sdk-swift/blob/main/Tests/ImmutableXCoreTests/Crypto/Stark/StarkKeyTests.swift#L30
    auto res = Ethereum::accountPathFromAddress("0xa76e3eeb2f7143165618ab8feaabcd395b6fac7f", internal::gLayer, internal::gApplication, internal::gIndex);
    ASSERT_EQ(res, "m/2645'/579218131'/211006541'/1534045311'/1431804530'/1");
}

TEST(ImmutableX, ExtraGrinding) {
    using namespace internal;
    std::string signature = "0x6d1550458c7a9a1257d73adbcf0fabc12f4497e970d9fa62dd88bf7d9e12719148c96225c1402d8707fd061b1aae2222bdf13571dfc82b3aa9974039f247f2b81b";
    std::string address = "0xa4864d977b944315389d1765ffa7e66F74ee8cd7";
    auto data = parse_hex(signature);
    auto path = DerivationPath(Ethereum::accountPathFromAddress(address, gLayer, gApplication, gIndex));
    auto privKey = ImmutableX::getPrivateKeyFromRawSignature(parse_hex(signature), path);
    auto pubKey = privKey.getPublicKey(TWPublicKeyTypeStarkex);
    ASSERT_EQ(hexEncoded(pubKey.bytes), "0x035919acd61e97b3ecdc75ff8beed8d1803f7ea3cad2937926ae59cc3f8070d4");
}

TEST(ImmutableX, GrindKey) {
    auto seed = parse_hex("86F3E7293141F20A8BAFF320E8EE4ACCB9D4A4BF2B4D295E8CEE784DB46E0519");
    auto res = grindKey(seed);
    ASSERT_EQ(res, "5c8c8683596c732541a59e03007b2d30dbbbb873556fe65b5fb63c16688f941");
}

TEST(ImmutableX, GetPrivateKeySignature) {
    std::string signature = "0x21fbf0696d5e0aa2ef41a2b4ffb623bcaf070461d61cf7251c74161f82fec3a4370854bc0a34b3ab487c1bc021cd318c734c51ae29374f2beb0e6f2dd49b4bf41c";
    auto data = parse_hex(signature);
    // The signature is `rsv`, where `r` starts at 0 and is 32 long.
    auto seed = subData(data, 0, 32);
    auto result = grindKey(seed);
    ASSERT_EQ(result, "766f11e90cd7c7b43085b56da35c781f8c067ac0d578eabdceebc4886435bda");
}

TEST(ImmutableX, GetPrivateKeyFromSignature) {
    using namespace internal;
    std::string address = "0xa76e3eeb2f7143165618ab8feaabcd395b6fac7f";
    std::string signature = "0x5a263fad6f17f23e7c7ea833d058f3656d3fe464baf13f6f5ccba9a2466ba2ce4c4a250231bcac7beb165aec4c9b049b4ba40ad8dd287dc79b92b1ffcf20cdcf1b";
    auto path = DerivationPath(Ethereum::accountPathFromAddress(address, gLayer, gApplication, gIndex));
    auto privKey = ImmutableX::getPrivateKeyFromRawSignature(parse_hex(signature), path);
    ASSERT_EQ(hex(privKey.bytes), "058ab7989d625b1a690400dcbe6e070627adedceff7bd196e58d4791026a8afe");
    ASSERT_TRUE(PrivateKey::isValid(privKey.bytes));
}

TEST(ImmutableX, GetPublicKeyFromPrivateKey) {
    auto privKeyData = parse_hex("058ab7989d625b1a690400dcbe6e070627adedceff7bd196e58d4791026a8afe", true);
    PrivateKey privKey(privKeyData);
    auto pubKey = privKey.getPublicKey(TWPublicKeyTypeStarkex);
    auto pubKeyHex = hexEncoded(pubKey.bytes);
    ASSERT_EQ(pubKeyHex, "0x02a4c7332c55d6c1c510d24272d1db82878f2302f05b53bcc38695ed5f78fffd");
}

TEST(ImmutableX, SimpleSign) {
    auto privKeyBytes = parse_hex("0139fe4d6f02e666e86a6f58e65060f115cd3c185bd9e98bd829636931458f79");
    PrivateKey privKey(privKeyBytes);
    auto digest = parse_hex("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76");
    auto signature = hex(privKey.sign(digest, TWCurve::TWCurveStarkex));
    auto expectedSignature = "061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a";
    ASSERT_EQ(signature.size(), 128ULL);
    ASSERT_EQ(signature.substr(0, 64), "061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f");
    ASSERT_EQ(signature.substr(64, 64), "04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a");
    ASSERT_EQ(signature, expectedSignature);
}

TEST(ImmutableX, VerifySign) {
    // valid
    {
        auto pubKeyData = parse_hex("02c5dbad71c92a45cc4b40573ae661f8147869a91d57b8d9b8f48c8af7f83159");
        auto hash = parse_hex("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76");
        auto signature = parse_hex("061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9a");
        auto pubKey = PublicKey(pubKeyData, TWPublicKeyTypeStarkex);
        ASSERT_TRUE(pubKey.verify(signature, hash));
    }
    // invalid
    {
        auto pubKeyData = parse_hex("02c5dbad71c92a45cc4b40573ae661f8147869a91d57b8d9b8f48c8af7f83159");
        auto hash = parse_hex("06fea80189363a786037ed3e7ba546dad0ef7de49fccae0e31eb658b7dd4ea76");
        auto signature = parse_hex("061ec782f76a66f6984efc3a1b6d152a124c701c00abdd2bf76641b4135c770f04e44e759cea02c23568bb4d8a09929bbca8768ab68270d50c18d214166ccd9b");
        auto pubKey = PublicKey(pubKeyData, TWPublicKeyTypeStarkex);
        ASSERT_FALSE(pubKey.verify(signature, hash));
    }
}

} // namespace TW::ImmutableX::tests
