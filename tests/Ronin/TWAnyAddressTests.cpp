// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWCoinType.h>

#include "Ronin/Address.h"
#include "Ronin/Entry.h"

#include <gtest/gtest.h>

const auto normalized1 = STRING("ronin:ec49280228b0d05aa8e8b756503254e1ee7835ab");

TEST(RoninAnyAddress, ValidateRoninNonChecksummed) {
    EXPECT_TRUE(TWAnyAddressIsValid(normalized1.get(), TWCoinTypeRonin));

    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(normalized1.get(), TWCoinTypeRonin));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string2.get(), normalized1.get()));

    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "ec49280228b0d05aa8e8b756503254e1ee7835ab");
}

TEST(RoninAnyAddress, ValidateRoninChecksummed) {
    auto checksummed = STRING("ronin:EC49280228b0D05Aa8e8b756503254e1eE7835ab");
    EXPECT_TRUE(TWAnyAddressIsValid(checksummed.get(), TWCoinTypeRonin));

    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(checksummed.get(), TWCoinTypeRonin));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string2.get(), normalized1.get()));

    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "ec49280228b0d05aa8e8b756503254e1ee7835ab");    
}

TEST(RoninAnyAddress, ValidateEthereum) {
    auto ethereum = STRING("0xEC49280228b0D05Aa8e8b756503254e1eE7835ab");
    EXPECT_TRUE(TWAnyAddressIsValid(ethereum.get(), TWCoinTypeRonin));

    auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(ethereum.get(), TWCoinTypeRonin));
    auto string2 = WRAPS(TWAnyAddressDescription(addr.get()));
    EXPECT_TRUE(TWStringEqual(string2.get(), normalized1.get()));

    auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
    assertHexEqual(keyHash, "ec49280228b0d05aa8e8b756503254e1ee7835ab");
}

TEST(RoninAnyAddress, Invalid) {
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("EC49280228b0D05Aa8e8b756503254e1eE7835ab").get(), TWCoinTypeRonin)); // no prefix
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("EC49280228b0D05Aa8e8b756503254e1eE7835").get(), TWCoinTypeRonin)); // too short
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("ronin:EC49280228b0D05Aa8e8b756503254e1eE7835").get(), TWCoinTypeRonin)); // too short
    EXPECT_FALSE(TWAnyAddressIsValid(STRING("ronin:ec49280228b0d05aa8e8b756503254e1ee7835").get(), TWCoinTypeRonin)); // too short
}
