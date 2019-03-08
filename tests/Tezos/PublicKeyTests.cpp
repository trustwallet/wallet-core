// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/BinaryCoding.h"
#include "PublicKey.h"
#include "Data.h"
#include "Tezos/Transaction.h"

#include <gtest/gtest.h>

#include <iostream>

using namespace TW::Tezos;

TEST(TezosPublicKey, forge) {
    auto input = parsePublicKey("edpkuAfEJCEatRgFpRGg3gn3FdWniLXBoubARreRwuVZPWufkgDBvR");
    auto expected = "00429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e";
    std::cout << forgePublicKey(input) << std::endl;
    for (auto key : input.bytes) {
      std::cout << (int) key << " ";
    }

    // ASSERT_EQ(input.forge(), expected);
}

TEST(TezosPublicKey, parse) {
    auto input = "edpkuAfEJCEatRgFpRGg3gn3FdWniLXBoubARreRwuVZPWufkgDBvR";
    auto bytes = Data({1, 69, 27, 222, 131, 36, 84, 186, 115, 230, 224, 222, 49, 63, 207, 93, 21, 101, 236, 81, 8, 14, 220, 115, 187, 25, 40, 123, 142, 10, 178, 18, 43});
    auto output = parsePublicKey(input);
    auto expected = TW::PublicKey(bytes);
    ASSERT_EQ(output, expected);
}
