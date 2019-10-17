// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TON/Address.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::TON;

static string TestGiverHex = "8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d";
static string TestGiverRaw = "-1:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d";
static string TestGiverUser = "Ef+BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODSkb";
static string TestGiverUserUrl = "Ef-BVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODSkb"; // base64url

TEST(TONAddress, WorkchainValid)
{
    ASSERT_TRUE(Workchain::isValid(0));
    ASSERT_TRUE(Workchain::isValid(-1));
    ASSERT_FALSE(Workchain::isValid(1));
    ASSERT_FALSE(Workchain::isValid(10));
}

TEST(TONAddress, AddressValidUser)
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

TEST(TONAddress, AddressValidRaw)
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

TEST(TONAddress, AddressFromUser)
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

TEST(TONAddress, AddressFromRaw)
{
    auto addr = Address(TestGiverRaw);
    // convert back to string, check
    auto addr2strUser = addr.string();
    ASSERT_EQ(TestGiverUser, addr2strUser);
    auto addr2strRaw = addr.stringRaw();
    ASSERT_EQ(TestGiverRaw, addr2strRaw);
}

TEST(TONAddress, AddressToString)
{
    auto addr = Address(TestGiverUser);

    auto strUser = addr.string();
    ASSERT_EQ(TestGiverUser, strUser);

    auto strUserRaw = addr.stringRaw();
    ASSERT_EQ(TestGiverRaw, strUserRaw);
}

TEST(TONAddress, AddressFromPublicKey)
{
    // Sample taken from TON HOWTO
    const auto publicKey = PublicKey(parse_hex("F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ("-1:60c04141c6a7b96d68615e7a91d265ad0f3a9a922e9ae9c901d4fa83f5d3c0d0", address.stringRaw());
    ASSERT_EQ("Ef9gwEFBxqe5bWhhXnqR0mWtDzqaki6a6ckB1PqD9dPA0KTM", address.string());
    // to match options 7 (non-bounceable, test only, base64url)
    address.isBounceable = false;
    address.isTestOnly = true;
    ASSERT_EQ("0f9gwEFBxqe5bWhhXnqR0mWtDzqaki6a6ckB1PqD9dPA0EKD", address.string());
}
