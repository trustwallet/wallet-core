// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "OasisProtocol/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::OasisProtocol;

TEST(OasisProtocolAddress, Valid) {
    ASSERT_TRUE(Address::isValid("oasis1qp0cnmkjl22gky6p6qeghjytt4v7dkxsrsmueweh"));
}

TEST(OasisProtocolAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("oasis1qp0cnmkjl22gky6p6qeghjytt4v7dkxsrsmuewehj"));
    ASSERT_FALSE(Address::isValid("oasi1qp0cnmkjl22gky6p6qeghjytt4v7dkxsrsmueweh"));
}

TEST(OasisProtocolAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("4f8b5676990b00e23d9904a92deb8d8f428ff289c8939926358f1d20537c21a0"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    ASSERT_EQ(address.string(), "oasis1qql6ra3pawzkhra4ejh98t0dsg7z8ysghgw8mw6m");
}

TEST(OasisProtocolAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("aba52c0dcb80c2fe96ed4c3741af40c573a0500c0d73acda22795c37cb0f1739"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "oasis1qp0cnmkjl22gky6p6qeghjytt4v7dkxsrsmueweh");
}

TEST(OasisProtocolAddress, FromString) {
    Address address;
    ASSERT_TRUE(Address::decode("oasis1qp0cnmkjl22gky6p6qeghjytt4v7dkxsrsmueweh", address));
    ASSERT_EQ(address.string(), "oasis1qp0cnmkjl22gky6p6qeghjytt4v7dkxsrsmueweh");

    ASSERT_FALSE(Address::decode("oasis1qp0cnmkjl22gky6p6qeghjytt4v7dkxsrsmuewehj", address));
}
