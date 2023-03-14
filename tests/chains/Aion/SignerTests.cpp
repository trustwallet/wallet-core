// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Aion/Signer.h"
#include "Aion/Transaction.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Aion::tests {

TEST(AionSigner, Sign) {
    auto address = Aion::Address("0xa082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e");
    auto transaction = Transaction(9, 20000000000, 21000, address, 10000, 155157377101, {});

    auto privateKey = PrivateKey(parse_hex("db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9"));
    Signer::sign(privateKey, transaction);

    EXPECT_EQ(hex(transaction.signature), "a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7d3d3386742c2716031b79950cef5fcb49c079a5cab095c8b08915e126b9741389924ba2d5c00036a3b39c2a8562fa0800f1a13a566ce6e027274ce63a41dec07");

    // Raw transaction
    EXPECT_EQ(hex(transaction.encode()), "f89b09a0a082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e8227108085242019b04d8252088800000004a817c80001b860a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7d3d3386742c2716031b79950cef5fcb49c079a5cab095c8b08915e126b9741389924ba2d5c00036a3b39c2a8562fa0800f1a13a566ce6e027274ce63a41dec07");
}

TEST(AionSigner, SignWithData) {
    auto address = Aion::Address("0xa082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e");
    auto transaction = Transaction(9, 20000000000, 21000, address, 10000, 155157377101, parse_hex("41494f4e0000"));

    auto privateKey = PrivateKey(parse_hex("db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9"));
    Signer::sign(privateKey, transaction);

    EXPECT_EQ(hex(transaction.signature), "a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a736fc2642c2d62900204779aa274dba3b8712eff7a8464aa78ea52b09ece20679fe3f5edf94c84a7e0c5f93213be891bc279af927086f455167f5bc73d3046c0d");

    // Raw transaction
    EXPECT_EQ(hex(transaction.encode()), "f8a109a0a082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e8227108641494f4e000085242019b04d8252088800000004a817c80001b860a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a736fc2642c2d62900204779aa274dba3b8712eff7a8464aa78ea52b09ece20679fe3f5edf94c84a7e0c5f93213be891bc279af927086f455167f5bc73d3046c0d");
}

} // namespace TW::Aion::tests
