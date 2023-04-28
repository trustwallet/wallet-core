// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Data.h"
#include "uint256.h"
#include <gtest/gtest.h>

namespace TW {

TEST(HexCoding, validation) {
    const std::string valid = "0x7d8bf18c7ce84b3e175b339c4ca93aed1dd166f1";
    const std::string invalid = "0xMQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4";
    const auto bytes = parse_hex(invalid);
    const auto bytes2 = parse_hex(valid);

    ASSERT_TRUE(bytes.empty());
    ASSERT_EQ("0x" + hex(bytes2), valid);
}

TEST(HexCoding, OddLength) {
    const std::string oddHex = "0x28fa6ae00";
    const auto bytes = parse_hex(oddHex, true);
    const auto number = load(bytes);
    ASSERT_EQ(number, 11000000000);
}

TEST(HexCoding, isHexEncoded) {
    ASSERT_TRUE(is_hex_encoded("66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3"));
    ASSERT_TRUE(is_hex_encoded("0x66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3"));
    ASSERT_FALSE(is_hex_encoded("1x66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3"));
    ASSERT_FALSE(is_hex_encoded("0xyahoo"));
}

}
