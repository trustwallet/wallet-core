// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Aion/Address.h"
#include "Aion/PrivateKey.h"
#include "Aion/PublicKey.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Aion;

TEST(AionAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7"));
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "a0d2312facea71b740679c926d040c9056a65a4bfa2ddd18ec160064f82909e7");
}