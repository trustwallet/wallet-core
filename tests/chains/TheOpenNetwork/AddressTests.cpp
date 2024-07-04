// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include "TestUtilities.h"

#include "TheOpenNetwork/Address.h"
#include "TheOpenNetwork/wallet/WalletV4R2.h"
#include "TheOpenNetwork/WorkchainType.h"

#include "TrustWalletCore/TWTONAddressConverter.h"

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

    ASSERT_EQ(address.string(), "UQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorhqg");
}

TEST(TheOpenNetworkAddress, FromPublicKeyV4R2) {
    const auto publicKey = PublicKey(parse_hex("c2036a1ca901059e1d1ab38cd7a7a4709b5e8f9d85b387f0514d7adae70b6afe"), TWPublicKeyTypeED25519);

    WalletV4R2 wallet(publicKey, WorkchainType::Basechain);
    const auto address = wallet.getAddress();

    ASSERT_EQ(address.string(), "UQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorhqg");
}

TEST(TheOpenNetworkAddress, GetJettonNotcoinAddress) {
    auto mainAddress = STRING("UQBjKqthWBE6GEcqb_epTRFrQ1niS6Z1Z1MHMwR-mnAYRoYr");
    auto addressBocEncoded = WRAPS(TWTONAddressConverterToBoc(mainAddress.get()));
    assertStringsEqual(addressBocEncoded, "te6ccgICAAEAAQAAACQAAABDgAxlVWwrAidDCOVN/vUpoi1oazxJdM6s6mDmYI/TTgMI0A==");

    // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
    // '{"address":"EQAvlWFDxGF2lXm67y4yzC17wYKD9A0guwPkMs1gOsM__NOT","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6ccgICAAEAAQAAACQAAABDgAxlVWwrAidDCOVN/vUpoi1oazxJdM6s6mDmYI/TTgMI0A=="]]}'

    // `get_wallet_address` response:
    auto jettonAddressBocEncoded = STRING("te6cckEBAQEAJAAAQ4AFvT5rqwxcbKfITqnkwL+go4Zi9bulRHAtLt4cjjFdK7B8L+Cq");
    auto jettonAddress = WRAPS(TWTONAddressConverterFromBoc(jettonAddressBocEncoded.get()));
    assertStringsEqual(jettonAddress, "UQAt6fNdWGLjZT5CdU8mBf0FHDMXrd0qI4FpdvDkcYrpXV5H");
}

TEST(TheOpenNetworkAddress, GetJettonUSDTAddress) {
    auto mainAddress = STRING("UQBjKqthWBE6GEcqb_epTRFrQ1niS6Z1Z1MHMwR-mnAYRoYr");
    auto addressBocEncoded = WRAPS(TWTONAddressConverterToBoc(mainAddress.get()));
    assertStringsEqual(addressBocEncoded, "te6ccgICAAEAAQAAACQAAABDgAxlVWwrAidDCOVN/vUpoi1oazxJdM6s6mDmYI/TTgMI0A==");

    // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
    // '{"address":"EQCxE6mUtQJKFnGfaROTKOt1lZbDiiX1kCixRv7Nw2Id_sDs","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6ccgICAAEAAQAAACQAAABDgAxlVWwrAidDCOVN/vUpoi1oazxJdM6s6mDmYI/TTgMI0A=="]]}'

    // `get_wallet_address` response:
    auto jettonAddressBocEncoded = STRING("te6cckEBAQEAJAAAQ4Aed71FEI46jdFXghsGUIG2GIR8wpbQaLzrKNj7BtHOEHBSO5Mf");
    auto jettonAddress = WRAPS(TWTONAddressConverterFromBoc(jettonAddressBocEncoded.get()));
    assertStringsEqual(jettonAddress, "UQDzveoohHHUboq8ENgyhA2wxCPmFLaDRedZRsfYNo5wg4TL");
}

TEST(TheOpenNetworkAddress, GetJettonStonAddress) {
    auto mainAddress = STRING("EQATQPeCwtMzQ9u54nTjUNcK4n_0VRSxPOOROLf_IE0OU3XK");
    auto addressBocEncoded = WRAPS(TWTONAddressConverterToBoc(mainAddress.get()));
    assertStringsEqual(addressBocEncoded, "te6ccgICAAEAAQAAACQAAABDgAJoHvBYWmZoe3c8TpxqGuFcT/6KopYnnHInFv/kCaHKcA==");

    // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
    // '{"address":"EQA2kCVNwVsil2EM2mB0SkXytxCqQjS4mttjDpnXmwG9T6bO","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6ccgICAAEAAQAAACQAAABDgAxlVWwrAidDCOVN/vUpoi1oazxJdM6s6mDmYI/TTgMI0A=="]]}'

    // `get_wallet_address` response:
    auto jettonAddressBocEncoded = STRING("te6cckEBAQEAJAAAQ4ALPu0dyA1gHd3r7J1rxlvhXSvT5y3rokMDMiCQ86TsUJDnt69H");
    auto jettonAddress = WRAPS(TWTONAddressConverterFromBoc(jettonAddressBocEncoded.get()));
    assertStringsEqual(jettonAddress, "UQBZ92juQGsA7u9fZOteMt8K6V6fOW9dEhgZkQSHnSdihHPH");
}

TEST(TheOpenNetworkAddress, FromBocNullAddress) {
    auto jettonAddressBocEncoded = STRING("te6cckEBAQEAAwAAASCUQYZV");
    auto jettonAddress = WRAPS(TWTONAddressConverterFromBoc(jettonAddressBocEncoded.get()));
    assertStringsEqual(jettonAddress, "UQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAJKZ");
}

TEST(TheOpenNetworkAddress, FromBocError) {
    // No type bit.
    auto boc1 = STRING("te6cckEBAQEAAwAAAcCO6ba2");
    ASSERT_EQ(TWTONAddressConverterFromBoc(boc1.get()), nullptr);

    // No res1 and workchain bits.
    auto boc2 = STRING("te6cckEBAQEAAwAAAaDsenDX");
    ASSERT_EQ(TWTONAddressConverterFromBoc(boc2.get()), nullptr);

    // Incomplete hash (31 bytes instead of 32).
    auto boc3 = STRING("te6cckEBAQEAIwAAQYAgQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAUGJnJWk=");
    ASSERT_EQ(TWTONAddressConverterFromBoc(boc3.get()), nullptr);

    // Expected 267 bits, found 268.
    auto boc4 = STRING("te6cckEBAQEAJAAAQ4AgQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEgGG0Gq");
    ASSERT_EQ(TWTONAddressConverterFromBoc(boc4.get()), nullptr);
}

TEST(TheOpenNetworkAddress, ToUserFriendly) {
    auto rawAddress = "0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae";
    auto bounceable = "EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorkdl";
    auto nonBounceable = "UQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorhqg";
    auto bounceableTestnet = "kQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorvzv";
    auto nonBounceableTestnet = "0QCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsorqEq";

    // Raw to user friendly.
    assertStringsEqual(
        WRAPS(TWTONAddressConverterToUserFriendly(STRING(rawAddress).get(), true, false)),
        bounceable
    );
    assertStringsEqual(
        WRAPS(TWTONAddressConverterToUserFriendly(STRING(rawAddress).get(), false, false)),
        nonBounceable
    );
    assertStringsEqual(
        WRAPS(TWTONAddressConverterToUserFriendly(STRING(rawAddress).get(), true, true)),
        bounceableTestnet
    );
    assertStringsEqual(
        WRAPS(TWTONAddressConverterToUserFriendly(STRING(rawAddress).get(), false, true)),
        nonBounceableTestnet
    );

    // Bounceable to non-bounceable.
    assertStringsEqual(
        WRAPS(TWTONAddressConverterToUserFriendly(STRING(bounceable).get(), false, false)),
        nonBounceable
    );

    // Non-bounceable to bounceable.
    assertStringsEqual(
        WRAPS(TWTONAddressConverterToUserFriendly(STRING(nonBounceable).get(), true, false)),
        bounceable
    );

    // Non-bounceable to non-bounceable.
    assertStringsEqual(
        WRAPS(TWTONAddressConverterToUserFriendly(STRING(nonBounceable).get(), false, false)),
        nonBounceable
    );
}

TEST(TheOpenNetworkAddress, ToUserFriendlyError) {
    // No "0:" prefix.
    auto invalid1 = STRING("8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae");
    ASSERT_EQ(TWTONAddressConverterToUserFriendly(invalid1.get(), true, false), nullptr);

    // Too short.
    auto invalid2 = STRING("EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsor");
    ASSERT_EQ(TWTONAddressConverterToUserFriendly(invalid1.get(), false, false), nullptr);
}

} // namespace TW::TheOpenNetwork::tests
