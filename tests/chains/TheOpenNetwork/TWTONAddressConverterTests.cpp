// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include "TrustWalletCore/TWTONAddressConverter.h"

namespace TW::TheOpenNetwork::tests {

TEST(TWTONAddressConverter, GetJettonNotcoinAddress) {
    auto mainAddress = STRING("UQBjKqthWBE6GEcqb_epTRFrQ1niS6Z1Z1MHMwR-mnAYRoYr");
    auto addressBocEncoded = WRAPS(TWTONAddressConverterToBoc(mainAddress.get()));
    assertStringsEqual(addressBocEncoded, "te6cckEBAQEAJAAAQ4AMZVVsKwInQwjlTf71KaItaGs8SXTOrOpg5mCP004DCNAptHQU");

    // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
    // '{"address":"EQAvlWFDxGF2lXm67y4yzC17wYKD9A0guwPkMs1gOsM__NOT","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6ccgICAAEAAQAAACQAAABDgAxlVWwrAidDCOVN/vUpoi1oazxJdM6s6mDmYI/TTgMI0A=="]]}'

    // `get_wallet_address` response:
    auto jettonAddressBocEncoded = STRING("te6cckEBAQEAJAAAQ4AFvT5rqwxcbKfITqnkwL+go4Zi9bulRHAtLt4cjjFdK7B8L+Cq");
    auto jettonAddress = WRAPS(TWTONAddressConverterFromBoc(jettonAddressBocEncoded.get()));
    assertStringsEqual(jettonAddress, "UQAt6fNdWGLjZT5CdU8mBf0FHDMXrd0qI4FpdvDkcYrpXV5H");
}

TEST(TWTONAddressConverter, GetJettonUSDTAddress) {
    auto mainAddress = STRING("UQBjKqthWBE6GEcqb_epTRFrQ1niS6Z1Z1MHMwR-mnAYRoYr");
    auto addressBocEncoded = WRAPS(TWTONAddressConverterToBoc(mainAddress.get()));
    assertStringsEqual(addressBocEncoded, "te6cckEBAQEAJAAAQ4AMZVVsKwInQwjlTf71KaItaGs8SXTOrOpg5mCP004DCNAptHQU");

    // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
    // '{"address":"EQCxE6mUtQJKFnGfaROTKOt1lZbDiiX1kCixRv7Nw2Id_sDs","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6ccgICAAEAAQAAACQAAABDgAxlVWwrAidDCOVN/vUpoi1oazxJdM6s6mDmYI/TTgMI0A=="]]}'

    // `get_wallet_address` response:
    auto jettonAddressBocEncoded = STRING("te6cckEBAQEAJAAAQ4Aed71FEI46jdFXghsGUIG2GIR8wpbQaLzrKNj7BtHOEHBSO5Mf");
    auto jettonAddress = WRAPS(TWTONAddressConverterFromBoc(jettonAddressBocEncoded.get()));
    assertStringsEqual(jettonAddress, "UQDzveoohHHUboq8ENgyhA2wxCPmFLaDRedZRsfYNo5wg4TL");
}

TEST(TWTONAddressConverter, GetJettonStonAddress) {
    auto mainAddress = STRING("EQATQPeCwtMzQ9u54nTjUNcK4n_0VRSxPOOROLf_IE0OU3XK");
    auto addressBocEncoded = WRAPS(TWTONAddressConverterToBoc(mainAddress.get()));
    assertStringsEqual(addressBocEncoded, "te6cckEBAQEAJAAAQ4ACaB7wWFpmaHt3PE6cahrhXE/+iqKWJ5xyJxb/5AmhynDu6Ygj");

    // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
    // '{"address":"EQA2kCVNwVsil2EM2mB0SkXytxCqQjS4mttjDpnXmwG9T6bO","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6ccgICAAEAAQAAACQAAABDgAxlVWwrAidDCOVN/vUpoi1oazxJdM6s6mDmYI/TTgMI0A=="]]}'

    // `get_wallet_address` response:
    auto jettonAddressBocEncoded = STRING("te6cckEBAQEAJAAAQ4ALPu0dyA1gHd3r7J1rxlvhXSvT5y3rokMDMiCQ86TsUJDnt69H");
    auto jettonAddress = WRAPS(TWTONAddressConverterFromBoc(jettonAddressBocEncoded.get()));
    assertStringsEqual(jettonAddress, "UQBZ92juQGsA7u9fZOteMt8K6V6fOW9dEhgZkQSHnSdihHPH");
}

TEST(TWTONAddressConverter, FromBocNullAddress) {
    auto jettonAddressBocEncoded = STRING("te6cckEBAQEAAwAAASCUQYZV");
    auto jettonAddress = WRAPS(TWTONAddressConverterFromBoc(jettonAddressBocEncoded.get()));
    assertStringsEqual(jettonAddress, "UQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAJKZ");
}

TEST(TWTONAddressConverter, FromBocError) {
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

TEST(TWTONAddressConverter, ToUserFriendly) {
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

TEST(TWTONAddressConverter, ToUserFriendlyError) {
    // No "0:" prefix.
    auto invalid1 = STRING("8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae");
    ASSERT_EQ(TWTONAddressConverterToUserFriendly(invalid1.get(), true, false), nullptr);

    // Too short.
    auto invalid2 = STRING("EQCKhieGGl3ZbJ2zzggHsSLaXtRzk0znVopbSxw2HLsor");
    ASSERT_EQ(TWTONAddressConverterToUserFriendly(invalid1.get(), false, false), nullptr);
}

} // namespace TW::TheOpenNetwork::tests
