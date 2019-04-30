// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bravo/Address.h"
#include "Bitshares/Address.h"

#include <gtest/gtest.h>

using Address = TW::Bravo::Address;

static const std::string bitsharesPrefix = TW::Bitshares::AddressPrefix;
static const std::vector<std::string> validPrefixes = {bitsharesPrefix};

TEST(BitsharesAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("BTS5LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUmm", validPrefixes));
    ASSERT_FALSE(Address::isValid("BTS115LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUm", validPrefixes));
}

TEST(BitsharesAddress, Base58) {
    ASSERT_EQ(
        Address("BTS5LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUm", validPrefixes).string(),
        "BTS5LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUm"
    );
    ASSERT_EQ(
        Address("BTS8Z6A8JeF4JMRaVAVcBxCE5NhtXz1WVHr6u1ckUnqqyc7HiTF4n", validPrefixes).string(),
        "BTS8Z6A8JeF4JMRaVAVcBxCE5NhtXz1WVHr6u1ckUnqqyc7HiTF4n"
    );
}

TEST(BitsharesAddress, IsValid) {
    ASSERT_TRUE(Address::isValid("BTS4yPnD1zXSqyK4LFHkt1d5m5LZq2oaMFGYjosVXURUfqaUAXTKQ", validPrefixes));
    ASSERT_TRUE(Address::isValid("BTS67eD8Rx4us2bjjxyc1ciWzP1MQJd8CvQDmiiqRnu5659LXMJiv", validPrefixes));
}