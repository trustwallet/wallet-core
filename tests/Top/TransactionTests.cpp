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
    auto transaction = Transaction(from, to, 1, 2, 3, data("12345678"), data("hello world"), data("top unit test"));
    ASSERT_EQ(hex(transaction.encode()), "f8509466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a399003250102038831323334353637388b68656c6c6f20776f726c648d746f7020756e69742074657374");
}
