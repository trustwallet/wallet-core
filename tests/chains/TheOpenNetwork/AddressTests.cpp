// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <gtest/gtest.h>
#include <vector>

#include "TheOpenNetwork/Address.h"
#include "Everscale/WorkchainType.h"

#include "HexCoding.h"
#include "PublicKey.h"
#include "PrivateKey.h"

namespace TW::TheOpenNetwork::tests {

TEST(TheOpenNetworkAddress, Valid) {
//    ASSERT_TRUE(Address::isValid("__ADD_VALID_ADDRESS_HERE__"));

    // TODO: Add more tests
}

TEST(TheOpenNetworkAddress, Invalid) {
//    ASSERT_FALSE(Address::isValid("__ADD_INVALID_ADDRESS_HERE__"));

//     TODO: Add more tests
}

TEST(TheOpenNetworkAddress, FromString) {
//    auto address = Address("__ADD_VALID_ADDRESS_HERE__");
//    ASSERT_EQ(address.string(), "__ADD_SAME_VALID_ADDRESS_HERE__");
}

TEST(TheOpenNetworkAddress, FromPrivateKey) {
    const auto privateKey = PrivateKey(parse_hex("ff3ceb81a22c726e9d61d3f336fc783de5d60020972ca3abc27b99e3cf573a88"));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    const auto address = Address(publicKey, Everscale::WorkchainType::Basechain);
    ASSERT_EQ(address.string(), "0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae");
}

TEST(TheOpenNetworkAddress, FromPublicKey) {
    const auto publicKey = PublicKey(parse_hex("c2036a1ca901059e1d1ab38cd7a7a4709b5e8f9d85b387f0514d7adae70b6afe"), TWPublicKeyTypeED25519);
    const auto address = Address(publicKey, Everscale::WorkchainType::Basechain);
    ASSERT_EQ(address.string(), "0:8a8627861a5dd96c9db3ce0807b122da5ed473934ce7568a5b4b1c361cbb28ae");
}

} // namespace TW::TheOpenNetwork::tests
