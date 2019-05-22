// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ARK/Address.h"
#include "Base58.h"
#include "Data.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::ARK;

TEST(ARKAddress, fromPrivateKey) {
    auto key = PrivateKey(parse_hex("d8839c2432bfd0a67ef10a804ba991eabba19f154a3d707917681d45822a5712"));
    auto pubKey = key.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Address(pubKey);

    ASSERT_EQ(address.string(), "AGeYmgbg2LgGxRW2vNNJvQ88PknEJsYizC");
}

TEST(ARKAddress, fromString) {
    auto address = Address("AewxfHQobSc49a4radHp74JZCGP8LRe4xA");
    ASSERT_EQ(address.string(), "AewxfHQobSc49a4radHp74JZCGP8LRe4xA");
}

TEST(ARKAddress, validation) {
    auto prefixes = std::vector<Data>();
    prefixes.insert(prefixes.end(), Data({Address::prefix}));

    ASSERT_TRUE(Address::isValid(std::string("ARjdsayeC5q6xRo432Ru3F9Zcb73A5dfME"), prefixes));
    ASSERT_FALSE(Address::isValid(std::string("D61mfSggzbvQgTUe6JhYKH2doHaqJ3Dyib"), prefixes));
}
