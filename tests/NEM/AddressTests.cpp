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