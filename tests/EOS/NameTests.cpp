// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EOS/Name.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::EOS;

TEST(EOSName, Invalid) {
    ASSERT_THROW(Name(std::string(14, 'a')), std::invalid_argument);

    std::string invalidNames[] = {"Alice",  "alice16", "12345satoshis"};
    for(auto name: invalidNames) {
        ASSERT_FALSE(Name(name).string() == name);
    }
}

TEST(EOSName, Valid) {
    ASSERT_NO_THROW(Name(std::string(13, 'a')));

    std::string validName = "satoshis12345";
    ASSERT_EQ(Name(validName).string(), validName);
    Data buf;
    Name(validName).serialize(buf);
    ASSERT_EQ(hex(buf), "458608d8354cb3c1");
}