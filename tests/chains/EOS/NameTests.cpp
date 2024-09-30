// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "EOS/Name.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::EOS::tests {

TEST(EOSName, Invalid) {
    ASSERT_THROW(Name(std::string(14, 'a')), std::invalid_argument);

    std::string invalidNames[] = {"Alice", "alice16", "12345satoshis"};
    for (auto name : invalidNames) {
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

} // namespace TW::EOS::tests