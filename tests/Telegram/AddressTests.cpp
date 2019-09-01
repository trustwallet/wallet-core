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

TEST(TelegramAddress, WorkchainValid)
{
    ASSERT_TRUE(Workchain::isValid(0));
    ASSERT_TRUE(Workchain::isValid(-1));
    ASSERT_FALSE(Workchain::isValid(1));
    ASSERT_FALSE(Workchain::isValid(10));
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
}

TEST(TelegramAddress, AddressValidUser)
{
    ASSERT_TRUE(Address::isValid(TestGiverUser));
}
