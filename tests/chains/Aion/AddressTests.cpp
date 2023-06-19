// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Aion/Address.h"
#include "Coin.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

#include "TestUtilities.h"

using namespace TW;

namespace TW::Aion::tests {

TEST(AionAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("01a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "0xa0d2312facea71b740679c926d040c9056a65a4bfa2ddd18ec160064f82909e7");
}

TEST(AionAddress, FromString) {
    std::string aionAddress = "0xa0d2312facea71b740679c926d040c9056a65a4bfa2ddd18ec160064f82909e7";
    const auto address = Address(aionAddress);
    ASSERT_EQ(address.string(), aionAddress);
    ASSERT_ANY_THROW(Address("0xffff"));
}

TEST(AionAddress, InvalidFromData) {
    ASSERT_ANY_THROW(Address(parse_hex("0xffff")));
    auto aionAddress = parse_hex("0xa0d2312facea71b740679c926d040c9056a65a4bfa2ddd18ec160064f82909e7");
    [[maybe_unused]] auto res = Address(aionAddress);
}

TEST(AionAddress, isValid) {
    std::string validAddress = "0xa0d2312facea71b740679c926d040c9056a65a4bfa2ddd18ec160064f82909e7";
    std::string invalidAddress = "0xzzd2312facea71b740679c926d040c9056a65a4bfa2ddd18ec160064f82909e7";

    ASSERT_TRUE(Address::isValid(validAddress));
    ASSERT_EQ(Address(parse_hex(validAddress)).string(), validAddress);
    ASSERT_FALSE(Address::isValid(invalidAddress));
    EXPECT_EXCEPTION(Address(parse_hex(invalidAddress)), "Invalid address data");
}

} // namespace TW::Aion::tests
