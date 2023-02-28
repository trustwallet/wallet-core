// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <regex>
#include <Tezos/MessageSigner.h>
#include <gtest/gtest.h>

namespace TW::Tezos::tests {
    TEST(TezosMessageSigner, inputToPayload) {
        auto payload = Tezos::inputToPayload("Tezos Signed Message: testUrl 2023-02-08T10:36:18.454Z Hello World");
        ASSERT_EQ(payload, "05010000004254657a6f73205369676e6564204d6573736167653a207465737455726c20323032332d30322d30385431303a33363a31382e3435345a2048656c6c6f20576f726c64");
    }

    TEST(TezosMessageSigner, formatMessage) {
        auto formatMessage = Tezos::formatMessage("Hello World", "testUrl");
        std::regex regex("Tezos Signed Message: \\S+ \\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}\\.\\d{3}Z .+");
        ASSERT_TRUE(std::regex_match(formatMessage, regex));
    }
}
