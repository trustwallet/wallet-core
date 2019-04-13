// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Iocoin/Address.h"

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Iocoin;

TEST(IocoinAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("0321851014f96129e9d712aa55bbde85c3b084fca5e37b29e4e03c006479018d94"));
    const auto address = Address(publicKey, 103);
	    ASSERT_EQ(address.string(), "iaZQtBdd1RPKveBTPGHu9NXPyBh8bybsDz");
}

TEST(IocoinAddress, Valid) {
    ASSERT_TRUE(Address::isValid(std::string("iepioaEoo8mCXbXdqMkGt3jfktQXF67cTb")));
}


TEST(IocoinAddress, FromString) {
    const auto string = "iqvvB9e7cP5FfyJnqHmt6rzjCMYwZrRNcT";
    const auto address = Address(string);

    ASSERT_EQ(address.string(), string);
}

