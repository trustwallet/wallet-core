// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Tezos/BinaryCoding.h"
#include "Tezos/Forging.h"
#include "PublicKey.h"
#include "Data.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Tezos::tests {

TEST(TezosPublicKey, forge) {
    auto input = parsePublicKey("edpkuAfEJCEatRgFpRGg3gn3FdWniLXBoubARreRwuVZPWufkgDBvR");
    auto expected = "00451bde832454ba73e6e0de313fcf5d1565ec51080edc73bb19287b8e0ab2122b";
    auto serialized = forgePublicKey(input);
    ASSERT_EQ(hex(serialized), expected);
}

TEST(TezosPublicKey, parse) {
    auto input = "edpkuAfEJCEatRgFpRGg3gn3FdWniLXBoubARreRwuVZPWufkgDBvR";
    auto bytes = Data({1, 69, 27, 222, 131, 36, 84, 186, 115, 230, 224, 222, 49, 63, 207, 93, 21, 101, 236, 81, 8, 14, 220, 115, 187, 25, 40, 123, 142, 10, 178, 18, 43});
    auto output = parsePublicKey(input);
    auto expected = PublicKey(bytes, TWPublicKeyTypeED25519);
    ASSERT_EQ(output, expected);
}

} // namespace TW::Tezos::tests
