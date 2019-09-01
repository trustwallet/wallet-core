// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Telegram/Address.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Telegram;

static string TestGiverHex = "8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d";
static string TestGiverRaw = "-1:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d";
static string TestGiverUser = "Ef+BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODSkb";
static string TestGiverUserUrl = "Ef-BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODSkb"; // base64url

TEST(TelegramAddress, WorkchainValid)
{
    ASSERT_TRUE(Workchain::isValid(0));
    ASSERT_TRUE(Workchain::isValid(-1));
    ASSERT_FALSE(Workchain::isValid(1));
    ASSERT_FALSE(Workchain::isValid(10));
}

TEST(TelegramAddress, AddressValidUser)
{
    ASSERT_TRUE(Address::isValid(TestGiverUser));
    // wrong length
    ASSERT_FALSE(Address::isValid("Ef+BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODSk"));
    ASSERT_FALSE(Address::isValid("Ef+BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODSkbz"));
    ASSERT_FALSE(Address::isValid("E"));
    ASSERT_FALSE(Address::isValid(""));
    // Wrong CRC: same value, but with invalid CRC (0)
    ASSERT_FALSE(Address::isValid("Ef+BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODQAA"));
}

TEST(TelegramAddress, AddressValidRaw)
{
    ASSERT_TRUE(Address::isValid(TestGiverRaw));
    ASSERT_TRUE(Address::isValid("0:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d"));
    // no colon
    ASSERT_FALSE(Address::isValid("-1 8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d"));
    // invalid workchainID
    ASSERT_FALSE(Address::isValid("9:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d"));
    ASSERT_FALSE(Address::isValid("A:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d"));
    ASSERT_FALSE(Address::isValid(":8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d"));
    // invalid address part
    ASSERT_FALSE(Address::isValid("0:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0dd"));
    ASSERT_FALSE(Address::isValid("0:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0"));
    ASSERT_FALSE(Address::isValid("0:"));
    ASSERT_FALSE(Address::isValid("0:0"));
    ASSERT_FALSE(Address::isValid(""));
}

TEST(TelegramAddress, AddressFromUser)
{
    {
        auto addr = Address(TestGiverUser);
        // verify fields
        ASSERT_TRUE(addr.isBounceable);
        ASSERT_FALSE(addr.isTestOnly);
        // convert back to string, check
        auto addr2strUser = addr.string();
        ASSERT_EQ(TestGiverUser, addr2strUser);
        auto addr2strRaw = addr.stringRaw();
        ASSERT_EQ(TestGiverRaw, addr2strRaw);
    }

    {
        // Base64Url format
        auto addr2 = Address(TestGiverUserUrl);
        // convert back to string, will be standard again
        auto addr22strUser = addr2.string();
        ASSERT_EQ(TestGiverUser, addr22strUser);
    }
}

TEST(TelegramAddress, AddressFromRaw)
{
    auto addr = Address(TestGiverRaw);
    // convert back to string, check
    auto addr2strUser = addr.string();
    ASSERT_EQ(TestGiverUser, addr2strUser);
    auto addr2strRaw = addr.stringRaw();
    ASSERT_EQ(TestGiverRaw, addr2strRaw);
}

TEST(TelegramAddress, AddressToString)
{
    auto addr = Address(TestGiverUser);

    auto strUser = addr.string();
    ASSERT_EQ(TestGiverUser, strUser);

    auto strUserRaw = addr.stringRaw();
    ASSERT_EQ(TestGiverRaw, strUserRaw);
}

TEST(TelegramAddress, AddressFromPublicKey)
{
    const auto publicKey = PublicKey(parse_hex("F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3"), TWPublicKeyTypeED25519);
    const auto address = Address(publicKey);
    ASSERT_EQ("Ef/i1CkT9THzhPebXBgRvAhspHRUXT8wC+1Hz+8xZtv1gs+k", address.string());
}
