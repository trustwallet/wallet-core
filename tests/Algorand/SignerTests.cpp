// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Algorand/Address.h"
#include "Algorand/Signer.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Algorand;

TEST(AlgorandSigner, Sign) {
    auto key = PrivateKey(parse_hex("549e65cb92f58f84ac3be0391dd677732c7978acd3e80a93240ce75a1aacccb9"));
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto from = Address(publicKey);
    auto to = Address("UCE2U2JC4O4ZR6W763GUQCG57HQCDZEUJY4J5I6VYY4HQZUJDF7AKZO5GM");
    auto note = parse_hex("7b0cc8");
    auto transaction = Transaction(
        /* from */ from,
        /* to */ to,
        /* fee */ 2063,
        /* amount */ 847,
        /* first round */ 51,
        /* last round */ 61,
        /* note */ note,
        /* type */ "pay"
    );

    auto serialized = transaction.serialize();
    auto signature = Signer::sign(key, transaction);
    auto result = transaction.serialize(signature);

    ASSERT_EQ(hex(serialized),
              "89a3616d74cd034fa3666565cd080fa2667633a26768c420260b2009a093a8868b7958722fa5e546ede7"
              "ed17e4d85c4c78afb0452690edd2a26c763da46e6f7465c4037b0cc8a3726376c420a089aa6922e3b998"
              "fadff6cd4808ddf9e021e4944e389ea3d5c638786689197ea3736e64c4207b354a2617e27a15b0f883c3"
              "6a86c48e871ccf6b3d7947786fe9b1279476116fa474797065a3706179");
    ASSERT_EQ(hex(result),
              "82a3736967c440a3f3051a5d89b4f9b827fdb5c9f7909360957b550b96455991ee51c2da585377e300a8"
              "ee1a33797782bf9c90a2244c84ad2b00de4f669d42d03777dd6aa15e06a374786e89a3616d74cd034fa3"
              "666565cd080fa2667633a26768c420260b2009a093a8868b7958722fa5e546ede7ed17e4d85c4c78afb0"
              "452690edd2a26c763da46e6f7465c4037b0cc8a3726376c420a089aa6922e3b998fadff6cd4808ddf9e0"
              "21e4944e389ea3d5c638786689197ea3736e64c4207b354a2617e27a15b0f883c36a86c48e871ccf6b3d"
              "7947786fe9b1279476116fa474797065a3706179");
}
