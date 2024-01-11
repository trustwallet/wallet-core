// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Aion/Transaction.h"
#include "HexCoding.h"
#include <gtest/gtest.h>

namespace TW::Aion::tests {

TEST(AionTransaction, Encode) {
    auto address = Aion::Address("0xa082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e");
    auto transaction = Transaction(9, 20000000000, 21000, address, 10000, 155157377101, {});
    ASSERT_EQ(hex(transaction.encode()), "f83909a0a082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e8227108085242019b04d8252088800000004a817c80001");
}

TEST(AionTransaction, EncodeWithSignature) {
    auto address = Aion::Address("0xa082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e");
    auto transaction = Transaction(9, 20000000000, 21000, address, 10000, 155157377101, {});
    transaction.signature = parse_hex("a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7d3d3386742c2716031b79950cef5fcb49c079a5cab095c8b08915e126b9741389924ba2d5c00036a3b39c2a8562fa0800f1a13a566ce6e027274ce63a41dec07");
    ASSERT_EQ(hex(transaction.encode()), "f89b09a0a082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e8227108085242019b04d8252088800000004a817c80001b860a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7d3d3386742c2716031b79950cef5fcb49c079a5cab095c8b08915e126b9741389924ba2d5c00036a3b39c2a8562fa0800f1a13a566ce6e027274ce63a41dec07");
}

} // namespace TW::Aion::tests
