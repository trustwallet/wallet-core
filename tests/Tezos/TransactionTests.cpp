// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/Address.h"
#include "Tezos/BinaryCoding.h"
#include "Tezos/Transaction.h"
#include "HexCoding.h"

#include <gtest/gtest.h>
using namespace TW::Tezos;

TEST(TezosTransaction, forgePayment) {
    auto tx1 = Transaction(
        Address("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"),
        1272,
        30738,
        10100,
        257,
        1,
        Address("tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt"),
        operationtype::TRANSACTION
    );
    auto expected = "08000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e81020100008fb5cea62d147c696afd9a93dbce962f4c8a9c9100";
    auto serialized1 = tx1.forge();

    ASSERT_EQ(serialized1, parse_hex(expected));
}

TEST(TezosTransaction, forgeReveal) {
    auto tx1 = Transaction(
        Address("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"),
        1272,
        30738,
        10100,
        257,
        1,
        parsePublicKey("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A"),
        operationtype::REVEAL
    );
    auto expected = "07000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e";
    auto serialized1 = tx1.forge();

    ASSERT_EQ(serialized1, parse_hex(expected));
}
