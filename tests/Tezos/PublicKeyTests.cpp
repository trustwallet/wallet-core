// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/PublicKey.h"
#include "Tezos/Transaction.h"

#include <gtest/gtest.h>
using namespace TW::Tezos;

TEST(TezosPublicKey, forge) {
    auto input = PublicKey("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A");
    auto expected = "00429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e";

    ASSERT_EQ(input.forge(), expected);
}
