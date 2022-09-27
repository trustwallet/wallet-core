// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Aptos/TransactionPayload.h>
#include <gtest/gtest.h>

namespace TW::Aptos::tests {

TEST(AptosTransactionPayload, PayLoadBasis) {
    ModuleId module(gAddressOne, "coin");
    TransactionPayload payload = EntryFunction(module, "transfer", {}, {});
    ASSERT_EQ(std::get<EntryFunction>(payload).module().name(), "coin");
}

} // namespace TW::Aptos::tests
