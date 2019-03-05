// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"
#include "HexCoding.h"
#include "TWTestUtilities.h"

#include <gtest/gtest.h>

using namespace TW;

TEST(HashTests, blake2b) {
    auto content = std::string("Hello world");
    auto hashed = Hash::blake2b(content, 64);
    auto result = hex(hashed);

    ASSERT_EQ(result, std::string("6ff843ba685842aa82031d3f53c48b66326df7639a63d128974c5c14f31a0f33343a8c65551134ed1ae0f2b0dd2bb495dc81039e3eeb0aa1bb0388bbeac29183"));
}

TEST(HashTests, blake2b_Personal) {
    auto personal_string = std::string("MyApp Files Hash");
    auto personal_data = Data(personal_string.begin(), personal_string.end());
    auto content = std::string("the same content");
    auto hashed = Hash::blake2b(content, 32, personal_data);
    auto result = hex(hashed);

    ASSERT_EQ(result, std::string("20d9cd024d4fb086aae819a1432dd2466de12947831b75c5a30cf2676095d3b4"));
}
