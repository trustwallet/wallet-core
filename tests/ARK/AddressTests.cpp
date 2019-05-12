// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include <gtest/gtest.h>
#include "Data.h"
#include "HexCoding.h"
#include "ARK/Address.h"
#include "Base58.h"

using namespace TW;
using namespace TW::ARK;

TEST(ARKAddress, validation){
    Data address = Base58::bitcoin.decode("ARjdsayeC5q6xRo432Ru3F9Zcb73A5dfME");
    ASSERT_TRUE(Address::isValid(address));
    address = Base58::bitcoin.decode("DRjdsayeC5q6xRo432Ru3F9Zcb73A5dfME");
    ASSERT_FALSE(Address::isValid(address));
}

TEST(ARKAddress, creation){
    Address address = Address(Base58::bitcoin.decode("ARjdsayeC5q6xRo432Ru3F9Zcb73A5dfME"));
    ASSERT_EQ(address.address, "ARjdsayeC5q6xRo432Ru3F9Zcb73A5dfME");
    ASSERT_EQ(address.bytes(), Base58::bitcoin.decode("ARjdsayeC5q6xRo432Ru3F9Zcb73A5dfME"));
}