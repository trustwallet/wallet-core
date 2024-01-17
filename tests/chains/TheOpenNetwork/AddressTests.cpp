// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PublicKey.h"
#include "PrivateKey.h"

#include "TheOpenNetwork/Address.h"
#include "TheOpenNetwork/wallet/WalletV4R2.h"
#include "TheOpenNetwork/WorkchainType.h"

#include <gtest/gtest.h>

namespace TW::TheOpenNetwork::tests {

TEST(TheOpenNetworkAddress, Valid) {
    ASSERT_TRUE(Address::isValid("-1:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae"));
    ASSERT_TRUE(Address::isValid("0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae"));

    // user-friendly, b64urlsafe
    ASSERT_TRUE(Address::isValid("EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl"));
    ASSERT_TRUE(Address::isValid("EQBGqFmKe3oY8PChYN9g92ZEV2ybkzVB-hCiesQRn5mFnrNv"));
    ASSERT_TRUE(Address::isValid("Ef8JfFPRpHBV_tZpCurvxMJW69nt2js3SuGEWojGnOpCVPRe"));
    ASSERT_TRUE(Address::isValid("Ef_drj6m7jcME0fWTA-OwFC-6F0Le2SuOUQ6ibRc3Vz8HL8H"));

    // user-friendly, b64
    ASSERT_TRUE(Address::isValid("EQAN6Dr3vziti1Kp9D3aEFqJX4bBVfCaV57Z+9jwKTBXICv8"));
    ASSERT_TRUE(Address::isValid("EQCmGW+z+UL00FmnhWaMvJq/i86YY5GlJP3uJW19KC5Tzq4C"));
}

TEST(TheOpenNetworkAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("random string"));

    // invalid size
    ASSERT_FALSE(Address::isValid("EQIcIZpPoMnWXd8FbC1KaLtcyIgVUlwsbFK_3P6f5uf_YyzoE"));
    ASSERT_FALSE(Address::isValid("EQIcIZpPoMnWXd8FbC1KaLtcyIgVUlwsbFK_3P6f5uf_YyE"));

    // invalid size after decode
    ASSERT_FALSE(Address::isValid("EQIcIZpPoMnWXd8FbC1KaLtcyIgVUlwsbFK_3P6f5uf_Yyw="));

    // invalid workchain
    ASSERT_FALSE(Address::isValid("1:0ccd5119f27f7fe4614476c34f7e5e93c7ae098e577cf2012f8b8043165cb809"));
    ASSERT_FALSE(Address::isValid("EQEMzVEZ8n9_5GFEdsNPfl6Tx64Jjld88gEvi4BDFly4CSyl"));
    ASSERT_FALSE(Address::isValid("-2:e0e98cfcf743292298ad9e379a3c2e6401797b9cbfc0fe98b4e14fd0ce07ecdf"));
    ASSERT_FALSE(Address::isValid("Ef7g6Yz890MpIpitnjeaPC5kAXl7nL_A_pi04U_Qzgfs3-Cj"));

    // invalid tag
    ASSERT_FALSE(Address::isValid("MwCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorsn8")); // 0x33
    ASSERT_FALSE(Address::isValid("swCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsornJ2")); // 0x80 + 0x33

    // invalid crc
    ASSERT_FALSE(Address::isValid("EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsormVH")); // crc[a, b] = crc[b, a]
    ASSERT_FALSE(Address::isValid("EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorpcF")); // crc=0x9705
}

TEST(TheOpenNetworkAddress, FromString) {
    auto raw_address = Address("0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae");
    ASSERT_EQ(raw_address.string(), "0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae");
    ASSERT_FALSE(raw_address.isUserFriendly);

    auto raw_address_uppercase = Address("0:8A8627861A5DD96C9DB3CE0807B122DA5ED473934CE7568A5B4B1C361CBB28AE");
    ASSERT_EQ(raw_address_uppercase.string(), "0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae");
    ASSERT_FALSE(raw_address_uppercase.isUserFriendly);

    // 0x11
    auto bounceable_address = Address("EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl");
    ASSERT_EQ(bounceable_address.string(), "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl");
    ASSERT_TRUE(bounceable_address.isUserFriendly);
    ASSERT_TRUE(bounceable_address.isBounceable);
    ASSERT_FALSE(bounceable_address.isTestOnly);

    // 0x51
    auto non_bounceable_address = Address("UQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorhqg");
    ASSERT_EQ(non_bounceable_address.string(), "UQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorhqg");
    ASSERT_TRUE(non_bounceable_address.isUserFriendly);
    ASSERT_FALSE(non_bounceable_address.isBounceable);
    ASSERT_FALSE(non_bounceable_address.isTestOnly);

    // 0x11 | 0x80
    auto test_bounceable_address = Address("kQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorvzv");
    ASSERT_EQ(test_bounceable_address.string(), "kQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorvzv");
    ASSERT_TRUE(test_bounceable_address.isUserFriendly);
    ASSERT_TRUE(test_bounceable_address.isBounceable);
    ASSERT_TRUE(test_bounceable_address.isTestOnly);

    // 0x51 | 0x80
    auto test_non_bounceable_address = Address("0QCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorqEq");
    ASSERT_EQ(test_non_bounceable_address.string(), "0QCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorqEq");
    ASSERT_TRUE(test_non_bounceable_address.isUserFriendly);
    ASSERT_FALSE(test_non_bounceable_address.isBounceable);
    ASSERT_TRUE(test_non_bounceable_address.isTestOnly);
}

TEST(TheOpenNetworkAddress, FromPrivateKeyV4R2) {
    const auto privateKey = PrivateKey(parse_hex("ff3ceb81a22c726e9d61d3f336fc783de5d60020972ca3abc27b99e3cf573a88"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);

    WalletV4R2 wallet(publicKey, WorkchainType::Basechain);
    const auto address = wallet.getAddress();

    ASSERT_EQ(address.string(), "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl");
}

TEST(TheOpenNetworkAddress, FromPublicKeyV4R2) {
    const auto publicKey = PublicKey(parse_hex("c2036a1ca901059e1d1ab38cd7a7a4709b5e8f9d85b387f0514d7adae70b6afe"), TWPublicKeyTypeED25519);

    WalletV4R2 wallet(publicKey, WorkchainType::Basechain);
    const auto address = wallet.getAddress();

    ASSERT_EQ(address.string(), "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl");
}

} // namespace TW::TheOpenNetwork::tests
