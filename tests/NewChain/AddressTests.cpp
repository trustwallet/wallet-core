// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "NewChain/Address.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::NewChain;

TEST(NewChainAddress, Valid) {
    ASSERT_TRUE(Address::isValid("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"));
}

TEST(NewChainAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("aaeb60f3e94c9b9a09f33669435e7ef1beaed"));
    ASSERT_FALSE(Address::isValid("fB6916095ca1df60bB79Ce92cE3Ea74c37c5d359"));
}

TEST(NewChainAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("ea2a7f292949079272bf082b1301ce31cf49015622d48f0036777848705f2650"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeNIST256p1Extended));
    ASSERT_EQ(address.string(), "0xBB70ef6e424e611b044350f4A5ed57789F0BbE96");
}

TEST(NewChainAddress, FromString) {
    auto address = Address("0xC29F0fdB32dC2A9Bd10CacAaA98cD1d61b3521c6");
    ASSERT_EQ(address.string(), "0xC29F0fdB32dC2A9Bd10CacAaA98cD1d61b3521c6");
}
