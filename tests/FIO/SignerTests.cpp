// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "FIO/Signer.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::FIO;
using namespace std;

TEST(FIOSigner, Actor) {
    {
        const auto addr1 = "FIO6cDpi7vPnvRwMEdXtLnAmFwygaQ8CzD7vqKLBJ2GfgtHBQ4PPy";
        Address addr = Address(addr1);
        EXPECT_EQ(addr1, addr.string());

        uint64_t shortenedKey = Actor::shortenKey(addr.bytes);
        EXPECT_EQ(1518832697283783336U, shortenedKey);

        string name = Actor::name(shortenedKey);
        EXPECT_EQ("2odzomo2v4pec", name);

        string actor = Actor::actor(addr);
        EXPECT_EQ("2odzomo2v4pe", actor);
    }
    {
        const auto addr2 = "FIO7uMZoeei5HtXAD24C4yCkpWWbf24bjYtrRNjWdmGCXHZccwuiE";
        Address addr = Address(addr2);
        EXPECT_EQ(addr2, addr.string());

        string actor = Actor::actor(addr);
        EXPECT_EQ("hhq2g4qgycfb", actor);
    }
    {
        const auto addr2 = "FIO7bxrQUTbQ4mqcoefhWPz1aFieN4fA9RQAiozRz7FrUChHZ7Rb8";
        Address addr = Address(addr2);
        EXPECT_EQ(addr2, addr.string());

        string actor = Actor::actor(addr);
        EXPECT_EQ("5kmx4qbqlpld", actor);
    }
}
