// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bech32Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

using namespace TW;

TEST(Bech32Address, Valid) {
    ASSERT_TRUE(Bech32Address::isValid("bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", "bnb"));
    
    ASSERT_TRUE(Bech32Address::isValid("cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02", "cosmos"));
    ASSERT_TRUE(Bech32Address::isValid("cosmospub1addwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq", "cosmos"));
    ASSERT_TRUE(Bech32Address::isValid("cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08", "cosmos"));
    ASSERT_TRUE(Bech32Address::isValid("cosmosvalconspub1zcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa", "cosmos"));

    ASSERT_TRUE(Bech32Address::isValid("zil1fwh4ltdguhde9s7nysnp33d5wye6uqpugufkz7", "zil"));
}

TEST(Bech32Address, Invalid) {
    ASSERT_FALSE(Bech32Address::isValid("bnb1grpf0955h0ykzq3ar6nmum7y6gdfl6lxfn46h2"));

    ASSERT_FALSE(Bech32Address::isValid("cosmos1xsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"));
    ASSERT_FALSE(Bech32Address::isValid("cosmospub1xddwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq"));
    ASSERT_FALSE(Bech32Address::isValid("cosmosvaloper1xxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08"));
    ASSERT_FALSE(Bech32Address::isValid("cosmosvalconspub1xcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa"));

    ASSERT_FALSE(Bech32Address::isValid("0x91cddcebe846ce4d47712287eee53cf17c2cfb7"));
    ASSERT_FALSE(Bech32Address::isValid(""));
    ASSERT_FALSE(Bech32Address::isValid("0x"));
    ASSERT_FALSE(Bech32Address::isValid("91cddcebe846ce4d47712287eee53cf17c2cfb7"));
}
