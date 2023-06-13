// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PublicKey.h"

#include "Ontology/Address.h"
#include "Ontology/Signer.h"

#include <gtest/gtest.h>

namespace TW::Ontology::tests {

TEST(OntologyAddress, validation) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("abeb60f3e94c1b9a09f33669435e7ef12eacd"));
    ASSERT_FALSE(Address::isValid("abcb60f3e94c9b9a09f33669435e7ef1beaedads"));
    ASSERT_TRUE(Address::isValid("ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD"));
}

TEST(OntologyAddress, fromPubKey) {
    auto address = Address(
        PublicKey(parse_hex("031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486"), TWPublicKeyTypeSECP256k1));
    EXPECT_EQ("AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5", address.string());
}

TEST(OntologyAddress, fromString) {
    auto b58Str = "AYTxeseHT5khTWhtWX1pFFP1mbQrd4q1zz";
    auto address = Address(b58Str);
    EXPECT_EQ(b58Str, address.string());
    auto errB58Str = "AATxeseHT5khTWhtWX1pFFP1mbQrd4q1zz";
    ASSERT_THROW(new Address(errB58Str), std::runtime_error);
}

TEST(OntologyAddress, fromMultiPubKeys) {
    auto signer1 = Signer(PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646")));
    auto signer2 = Signer(PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464652")));
    auto signer3 = Signer(PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464658")));
    std::vector<Data> pubKeys{signer1.getPublicKey().bytes, signer2.getPublicKey().bytes, signer3.getPublicKey().bytes};
    uint8_t m = 2;
    auto multiAddress = Address(m, pubKeys);
    EXPECT_EQ("AYGWgijVZnrUa2tRoCcydsHUXR1111DgdW", multiAddress.string());
}

TEST(OntologyAddress, fromBytes) {
    auto address = Address(
        PublicKey(parse_hex("031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486"), TWPublicKeyTypeSECP256k1));
    EXPECT_EQ("AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5", address.string());

    std::vector<uint8_t> v(20);

    for (auto i = 0ul; i < address._data.size(); ++i) {
        v[i] = address._data[i];
    }
    auto address2 = Address(v);
    EXPECT_EQ("AeicEjZyiXKgUeSBbYQHxsU1X3V5Buori5", address2.string());

    v.pop_back();
    EXPECT_ANY_THROW(new Address(v));
}

} // namespace TW::Ontology::tests
