// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Top/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Top;

TEST(TopAddress, Valid) {
    ASSERT_TRUE(Address::isValid("T8000066ab344963eaa071f9636faac26b0d1a39900325"));

    // TODO: Add more tests
}

TEST(TopAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("T0000066ab344963eaa071f9636faac26b0d1a39900325"));

    // TODO: Add more tests
}

TEST(TopAddress, FromPrivateKey) {
    // TODO: Check public key type, finalize implementation

    auto privateKey = PrivateKey(parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    ASSERT_EQ(address.string(), "T8000066ab344963eaa071f9636faac26b0d1a39900325");
}

TEST(TopAddress, FromPublicKey) {
    // TODO: Check public key type, finalize implementation
    
    auto publicKey = PublicKey(parse_hex("0488227084554fae2b4b270cb56c9d8ee2b6bb1ee06863945ecce5bff9f8a197aa4546799dbe85decd6f3a43f52a2b06f77efd53af8e8530463d27c1b5945608bf"), TWPublicKeyTypeSECP256k1Extended);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "T8000066ab344963eaa071f9636faac26b0d1a39900325");
}

TEST(TopAddress, FromString) {
    auto address = Address("T8000066ab344963eaa071f9636faac26b0d1a39900325");
    ASSERT_EQ(address.string(), "T8000066ab344963eaa071f9636faac26b0d1a39900325");
}
