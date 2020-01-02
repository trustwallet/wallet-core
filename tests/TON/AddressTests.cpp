// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TON/Address.h"
#include "TON/Contract.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::TON;

static string TestGiverHex = "8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d";
static string TestGiverRaw = "0:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d";
static string TestGiverUser = "EQCBVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODdZT"; // base64url
static string TestGiverUserRegular = "EQCBVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODdZT"; // regular base64

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
    ASSERT_FALSE(Address::isValid("EQCBVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODdZ")); // shorter
    ASSERT_FALSE(Address::isValid("EQCBVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODdZTz")); // longer
    ASSERT_FALSE(Address::isValid("E"));
    ASSERT_FALSE(Address::isValid(""));
    // Wrong CRC: same value, but with invalid CRC (0)
    ASSERT_FALSE(Address::isValid("EQCBVndbeTJeXWLnQtm5bDC2UVpc0vH2TF2ksZPAPwcODd0T"));
}

TEST(TONAddress, AddressValidRaw)
{
    ASSERT_TRUE(Address::isValid(TestGiverRaw));
    ASSERT_TRUE(Address::isValid("0:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d"));
    // no colon
    ASSERT_FALSE(Address::isValid("0 8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d"));
    // invalid workchainID
    ASSERT_FALSE(Address::isValid("9:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d"));
    ASSERT_FALSE(Address::isValid("A:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d"));
    ASSERT_FALSE(Address::isValid(":8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d"));
    // valid, but not supported chainID
    ASSERT_FALSE(Address::isValid("-1:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0d"));
    // invalid address part
    ASSERT_FALSE(Address::isValid("0:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0dd")); // longer
    ASSERT_FALSE(Address::isValid("0:8156775b79325e5d62e742d9b96c30b6515a5cd2f1f64c5da4b193c03f070e0")); // shorter
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
        EXPECT_EQ(TestGiverUser, addr2strUser);
        auto addr2strRaw = addr.stringRaw();
        EXPECT_EQ(TestGiverRaw, addr2strRaw);
    }

    {
        // Base64Url format
        auto addr2 = Address(TestGiverUserRegular);
        // convert back to string, will be standard again
        auto addr22strUser = addr2.string();
        EXPECT_EQ(TestGiverUser, addr22strUser);
    }
}

TEST(TONAddress, AddressFromRaw)
{
    auto addr = Address(TestGiverRaw);
    // convert back to string, check
    auto addr2strUser = addr.string();
    EXPECT_EQ(TestGiverUser, addr2strUser);
    auto addr2strRaw = addr.stringRaw();
    EXPECT_EQ(TestGiverRaw, addr2strRaw);
}

TEST(TONAddress, AddressToString)
{
    auto addr = Address(TestGiverUser);

    auto strUser = addr.string();
    EXPECT_EQ(TestGiverUser, strUser);

    auto strUserRaw = addr.stringRaw();
    EXPECT_EQ(TestGiverRaw, strUserRaw);
}

TEST(TONAddress, createStateInit) {
    const auto publicKey = PublicKey(parse_hex("F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3"), TWPublicKeyTypeED25519);
    Cell stateInit = Contract::createStateInit(publicKey);
    EXPECT_EQ(2, stateInit.cellCount());
    if (stateInit.cellCount() >= 2) {
        // first cell contains smart contract code
        EXPECT_EQ(81, stateInit.getCells()[0]->getSlice().size());
        // second cell contains public key
        EXPECT_EQ("00000000f61cf0bc8e891ad7636e0cd35229d579323aa2da827eb85d8071407464dc2fa3", hex(stateInit.getCells()[1]->getSlice().data()));
    }
    auto hash = stateInit.hash();
    EXPECT_EQ("240090ab66459bf6e61e3dfd43f7b9c1f1e7d4bd81d3b2a4ac7323cc1a970753", hex(hash));
}

TEST(TONAddress, AddressFromPublicKey)
{
    // Sample taken from TON HOWTO
    const auto publicKey = PublicKey(parse_hex("F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);

    EXPECT_EQ("0:240090ab66459bf6e61e3dfd43f7b9c1f1e7d4bd81d3b2a4ac7323cc1a970753", address.stringRaw());
    EXPECT_EQ("EQAkAJCrZkWb9uYePf1D97nB8efUvYHTsqSscyPMGpcHUx3Y", address.string());
    // to match options 7 (non-bounceable, test only, base64url)
    address.isBounceable = false;
    address.isTestOnly = true;
    EXPECT_EQ("0QAkAJCrZkWb9uYePf1D97nB8efUvYHTsqSscyPMGpcHU_uX", address.string());

    // MasterChain
    address.workchainId = Workchain::MasterChainId;
    address.isBounceable = true;
    address.isTestOnly = false;
    EXPECT_EQ("-1:240090ab66459bf6e61e3dfd43f7b9c1f1e7d4bd81d3b2a4ac7323cc1a970753", address.stringRaw());
    EXPECT_EQ("Ef8kAJCrZkWb9uYePf1D97nB8efUvYHTsqSscyPMGpcHU-KQ", address.string());
    // to match options 7 (non-bounceable, test only, base64url)
    address.isBounceable = false;
    address.isTestOnly = true;
    EXPECT_EQ("0f8kAJCrZkWb9uYePf1D97nB8efUvYHTsqSscyPMGpcHUwTf", address.string());
}
