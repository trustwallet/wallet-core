// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "NEM/Address.h"
#include "NEM/Signer.h"

#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::NEM;

TEST(NEMAddress, IsValid) {
    // No address
    ASSERT_FALSE(Address::isValid(""));
    // Invalid checksum
    ASSERT_FALSE(Address::isValid("NQ42 2H8F YGU5 RM77 QSN9 LYLH C56A CYYR 0MLA"));
    // Too short
    ASSERT_FALSE(Address::isValid("NQ86 2H8F YGU5 RM77 QSN9 LYLH C56A CYYR 0ML"));
    // Too long
    ASSERT_FALSE(Address::isValid("NCKZD7 JGDLND IVVPH6 U2PG2Q KD3PX3 FX4CPZ-MF2A"));
    // Valid, without spaces
    ASSERT_TRUE(Address::isValid("NCKZD7JGDLNDIVVPH6U2PG2QKD3PX3FX4CPZMF2A"));
    // Valid, normal format
    ASSERT_TRUE(Address::isValid("NDYOM3 AM2MZ3 CMCVZ5 GVZEJ2 5QAYKR EVK53D 27L4"));
}

/*
TEST(NEMAddress, String) {
    // Address to string
    ASSERT_EQ(
        Address(parse_hex("5b3e9e5f32b89abafc3708765dc8f00216cefbb1")).string(),
        "NQ61 BCY9 UPRJ P2DB MY1P 11T5 TJ7G 08BC VXVH"
    );
    // Without spaces
    ASSERT_EQ(
        Address("NQ862H8FYGU5RM77QSN9LYLHC56ACYYR0MLA").string(),
        "NQ86 2H8F YGU5 RM77 QSN9 LYLH C56A CYYR 0MLA"
    );
    // With spaces
    ASSERT_EQ(
        Address("NQ86 2H8F YGU5 RM77 QSN9 LYLH C56A CYYR 0MLA").string(),
        "NQ86 2H8F YGU5 RM77 QSN9 LYLH C56A CYYR 0MLA"
    );
}

TEST(NEMAddress, FromPublicKey) {
    const auto publicKey = Signer::publicKeyFromBytes(
        parse_hex("70c7492aaa9c9ac7a05bc0d9c5db2dae9372029654f71f0c7f95deed5099b702"));
    const auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "NQ27 GBAY EVHP HK5X 6JHV JGFJ 5M3H BF4Y G7GD");
}
*/