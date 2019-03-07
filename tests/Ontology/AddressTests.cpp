// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.


#include "Ontology/Address.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Ontology;

TEST(OntologyAddress, Validation) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("abeb60f3e94c1b9a09f33669435e7ef12eacd"));
    ASSERT_TRUE(Address::isValid("abcb60f3e94c9b9a09f33669435e7ef1beaedads"));
}

TEST(OntologyAddress, fromPubKey) {
    auto address = Address(PublicKey(parse_hex("02a600c1bb0d195f9dd92d8465ce76e53c87be72032d9df8d3fd35540ace9bfc5b")));
    EXPECT_EQ("9f846c6abb6800fc6c61a488d4ed2f8688d416b7", address.hexString());
}

TEST(OntologyAddress, fromB58Str) {
    auto b58Str = "AYTxeseHT5khTWhtWX1pFFP1mbQrd4q1zz";
    EXPECT_EQ(b58Str, Address(b58Str).b58String());
    auto errB58Str = "AATxeseHT5khTWhtWX1pFFP1mbQrd4q1zz";
    EXPECT_THROW(new Address(errB58Str), std::runtime_error);
}

TEST(OntologyAddress, fromData) {
    auto vec = parse_hex("b716d488862fedd488a4616cfc0068bb6a6c849f");
    auto address = Address(vec);
    EXPECT_EQ("AYTxeseHT5khTWhtWX1pFFP1mbQrd4q1zz", address.b58String());
    auto errVec = parse_hex("b716d488862fedd488a4616cfc0068bb6a6c849f1");
    EXPECT_THROW(new Address(errVec), std::runtime_error);
}