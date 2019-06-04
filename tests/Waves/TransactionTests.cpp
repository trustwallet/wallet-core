// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Waves/Address.h"
#include "Waves/Transaction.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Waves;

TEST(WavesTransaction, serialize) {
    auto tx1 = Transaction(
        /* amount */ 100000000,
        /* amount asset */ "",
        /* fee */ 100000000,
        /* fee asset*/ Transaction::WAVES,
        /* to */ Address("3PLgzJXQiN77G7KgnR1WVa8jBYhF2dmWndx"),
        /* attachment */ parse_hex("66616c6166656c"),
        /* timestamp */ 1526641218066,
        /* pub_key */
        parse_hex("d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef22"));
    auto serialized1 = tx1.serializeToSign();
    ASSERT_EQ(hex(serialized1), "0402d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef"
                                "2200000000016372e852120000000005f5e1000000000005f5e1000157cdc9381c"
                                "071beb5abd27738d5cd36cf75f3cbfdd69e8e6bb000766616c6166656c");

    auto tx2 = Transaction(
        /* amount */ 1,
        /* amount asset */ "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq",
        /* fee */ 1,
        /* fee asset*/ "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq",
        /* to */ Address("3PLgzJXQiN77G7KgnR1WVa8jBYhF2dmWndx"),
        /* attachment */ Data(),
        /* timestamp */ 1,
        /* pub_key */
        parse_hex("d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef22"));
    auto serialized2 = tx2.serializeToSign();
    ASSERT_EQ(hex(serialized2),
              "0402d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef2201bae8ddc9955fa6"
              "f69f8e7b155efcdb97bc3bb3a95db4c4604408cec245cd187201bae8ddc9955fa6f69f8e7b155efcdb97"
              "bc3bb3a95db4c4604408cec245cd18720000000000000001000000000000000100000000000000010157"
              "cdc9381c071beb5abd27738d5cd36cf75f3cbfdd69e8e6bb0000");
}

TEST(WavesTransaction, failedSerialize) {
    // 141 bytes attachment
    auto tx1 = Transaction(
        /* amount */ 100000000,
        /* amount asset */ "",
        /* fee */ 100000000,
        /* fee asset*/ "",
        /* to */ Address("3PLgzJXQiN77G7KgnR1WVa8jBYhF2dmWndx"),
        /* attachment */
        parse_hex("66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166"
                  "656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c"
                  "6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66616c6166656c66"
                  "616c6166656c66616c6166656c66616c6166656c01"),
        /* timestamp */ 1526641218066,
        /* pub_key */
        parse_hex("d528aabec35ca100d87c7b7a128632faf19cd44531819457445113a32a21ef22"));
    EXPECT_THROW(tx1.serializeToSign(), invalid_argument);
}