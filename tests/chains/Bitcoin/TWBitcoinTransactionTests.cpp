// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/Transaction.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Bitcoin {

TEST(BitcoinTransaction, Encode) {
    auto transaction = Transaction(2, 0);

    auto po0 = OutPoint(parse_hex("5897de6bd6027a475eadd57019d4e6872c396d0716c4875a5f1a6fcfdf385c1f"), 0);
    transaction.inputs.emplace_back(po0, Script(), 4294967295);

    auto po1 = OutPoint(parse_hex("bf829c6bcf84579331337659d31f89dfd138f7f7785802d5501c92333145ca7c"), 18);
    transaction.inputs.emplace_back(po1, Script(), 4294967295);

    auto po2 = OutPoint(parse_hex("22a6f904655d53ae2ff70e701a0bbd90aa3975c0f40bfc6cc996a9049e31cdfc"), 1);
    transaction.inputs.emplace_back(po2, Script(), 4294967295);

    auto oscript0 = Script(parse_hex("76a9141fc11f39be1729bf973a7ab6a615ca4729d6457488ac"));
    transaction.outputs.emplace_back(18000000, oscript0);

    auto oscript1 = Script(parse_hex("0x76a914f2d4db28cad6502226ee484ae24505c2885cb12d88ac"));
    transaction.outputs.emplace_back(400000000, oscript1);

    Data unsignedData;
    transaction.encode(unsignedData, Transaction::SegwitFormatMode::NonSegwit);
    ASSERT_EQ(unsignedData.size(), 201ul);
    ASSERT_EQ(hex(unsignedData),
              "02000000035897de6bd6027a475eadd57019d4e6872c396d0716c4875a5f1a6fcfdf385c1f0000000000ffffffffbf829c6bcf84579331337659d31f89dfd138f7f7785802d5501c92333145ca7c1200000000ffffffff22a6f904655d53ae2ff70e701a0bbd90aa3975c0f40bfc6cc996a9049e31cdfc0100000000ffffffff0280a81201000000001976a9141fc11f39be1729bf973a7ab6a615ca4729d6457488ac0084d717000000001976a914f2d4db28cad6502226ee484ae24505c2885cb12d88ac00000000");
}

} // namespace TW::Bitcoin
