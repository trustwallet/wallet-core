// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "uint256.h"
#include "HexCoding.h"
#include "NEO/CoinReference.h"
#include <gtest/gtest.h>

namespace TW::NEO::tests {

using namespace std;

TEST(NEOCoinReference, Serialize) {
    auto coinReference = CoinReference();
    string prevHash = "bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a";
    coinReference.prevHash = load(parse_hex(prevHash));
    coinReference.prevIndex = 1;
    EXPECT_EQ(prevHash + "0100", hex(coinReference.serialize()));
}

TEST(NEOCoinReference, SerializeWithZeroLeading) {
    auto coinReference = CoinReference();
    string prevHash = "0037ebf259ca5c6c43a5e7117c910858ea1146290e07d39e48554bc00d890b94";
    coinReference.prevHash = load(parse_hex(prevHash));
    coinReference.prevIndex = 1;
    EXPECT_EQ(prevHash + "0100", hex(coinReference.serialize()));
}

TEST(NEOCoinReference, Deserialize) {
    auto coinReference = CoinReference();
    coinReference.deserialize(parse_hex("bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a0100"));
    EXPECT_EQ("bdecbb623eee6f9ade28d5a8ff5fb3ea9c9d73af039e0286201b3b0291fb4d4a", hex(store(coinReference.prevHash)));
    EXPECT_EQ(1, coinReference.prevIndex);
}

} // namespace TW::NEO::tests
