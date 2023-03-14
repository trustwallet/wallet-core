// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Theta/Transaction.h"

#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Theta::tests {

TEST(ThetaTransaction, Encode) {
    const auto from = Ethereum::Address("0x2E833968E5bB786Ae419c4d13189fB081Cc43bab");
    const auto to = Ethereum::Address("0x9F1233798E905E173560071255140b4A8aBd3Ec6");
    auto transaction = Transaction(from, to, 10, 20, 1);
    ASSERT_EQ(hex(transaction.encode()),
              "02f843c78085e8d4a51000e0df942e833968e5bb786ae419c4d13189fb081cc43babc70a85e8d4a51014"
              "0180d9d8949f1233798e905e173560071255140b4a8abd3ec6c20a14");
}

TEST(ThetaTransaction, EncodeWithSignature) {
    const auto from = Ethereum::Address("0x2E833968E5bB786Ae419c4d13189fB081Cc43bab");
    const auto to = Ethereum::Address("0x9F1233798E905E173560071255140b4A8aBd3Ec6");
    auto transaction = Transaction(from, to, 10, 20, 1);
    transaction.setSignature(
        from, parse_hex("5190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb7fff267cb8fe9134"
                        "ccdef053ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501"));
    ASSERT_EQ(hex(transaction.encode()),
              "02f887c78085e8d4a51000f863f861942e833968e5bb786ae419c4d13189fb081cc43babc70a85e8d4a5"
              "101401b8415190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb7fff267cb8fe9134"
              "ccdef053ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501d9d8949f1233798e905e17356007"
              "1255140b4a8abd3ec6c20a14");
}

} // namespace TW::Theta::tests
