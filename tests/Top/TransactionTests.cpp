// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Top/Transaction.h"

#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Top;

TEST(TopTransaction, Encode) {
    auto from = Top::Address("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    auto to = Top::Address("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    auto transaction = Transaction(from, to, 1, 2, 3, {}, {}, {});
    ASSERT_EQ(hex(transaction.encode()), "f09466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a39900325010203808080");
}

TEST(TopTransaction, EncodeWithData) {
    auto from = Top::Address("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    auto to = Top::Address("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    auto transaction = Transaction(from, to, 1, 2, 3, parse_hex("01020304"), parse_hex("05060708"), parse_hex("090a0b0c"));
    ASSERT_EQ(hex(transaction.encode()), "f83c9466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a399003250102038401020304840506070884090a0b0c");
}
