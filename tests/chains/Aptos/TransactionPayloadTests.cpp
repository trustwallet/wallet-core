// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Aptos/TransactionPayload.h>
#include <HexCoding.h>
#include <gtest/gtest.h>

namespace TW::Aptos::tests {

TEST(AptosTransactionPayload, PayLoadBasis) {
    ModuleId module(Address::one(), "coin");
    std::uint64_t amount{1000};
    Address to("0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b");
    BCS::Serializer serializer;
    serializer << to;
    std::vector<Data> args;
    args.emplace_back(serializer.bytes);
    serializer.clear();
    serializer << amount;
    args.emplace_back(serializer.bytes);
    TransactionPayload payload = EntryFunction(module, "transfer", {gTransferTag}, args);
    ASSERT_EQ(std::get<EntryFunction>(payload).module().name(), "coin");
    ASSERT_EQ(std::get<EntryFunction>(payload).module().shortString(), "0x1::coin");
    serializer.clear();
    serializer << payload;
    ASSERT_EQ(hex(serializer.bytes), "02000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e000220eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b08e803000000000000");
}

} // namespace TW::Aptos::tests
