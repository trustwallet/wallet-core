// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Aion/PrivateKey.h"
#include "Aion/PublicKey.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Aion;

TEST(AionPrivateKey, GetPublicKey) {
	auto privateKey = PrivateKey(parse_hex("db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7"));
	auto publicKey = privateKey.getPublicKey();
    ASSERT_EQ(hex(publicKey), "a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7");
}