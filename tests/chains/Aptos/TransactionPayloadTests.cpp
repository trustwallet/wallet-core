// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Aptos/TransactionPayload.h>
#include <HexCoding.h>
#include <gtest/gtest.h>

namespace TW::Aptos::tests {

TEST(AptosTransactionPayload, PancakeSwapPayload) {
    auto pancakeSwapPayload=R"(
    {
"arguments": [
  "0xc95db29a67a848940829b3df6119b5e67b788ff0248676e4484c7c6f29c0f5e6"
],
"function": "0xc23c3b70956ce8d88fb18ad9ed3b463fe873cb045db3f6d2e2fb15b9aab71d50::IFO::release",
"type": "entry_function_payload",
"type_arguments": [
  "0x48e0e3958d42b8d452c9199d4a221d0d1b15d14655787453dbe77208ced90517::coins::BUSD",
  "0x48e0e3958d42b8d452c9199d4a221d0d1b15d14655787453dbe77208ced90517::coins::DAI",
  "0x9936836587ca33240d3d3f91844651b16cb07802faf5e34514ed6f78580deb0a::uints::U1"
]
}
)"_json;

    TransactionPayload payload = EntryFunction::from_json(pancakeSwapPayload);
    BCS::Serializer serializer;
    Address sender("0x2ce519d8cd60e0870e874e8000e8cbc87c8172e6acdbec83662b4c8cc3fc3de9");
    std::uint64_t sequenceNumber{75};
    std::uint64_t gasAmount{488130};
    std::uint64_t gasPrice{100};
    std::uint64_t expirationTime{199940521552};
    std::uint8_t chainId{1};
    serializer << sender << sequenceNumber << payload << gasAmount << gasPrice << expirationTime << chainId;
    ASSERT_EQ(hex(serializer.bytes), "2ce519d8cd60e0870e874e8000e8cbc87c8172e6acdbec83662b4c8cc3fc3de94b0000000000000002c23c3b70956ce8d88fb18ad9ed3b463fe873cb045db3f6d2e2fb15b9aab71d500349464f0772656c65617365030748e0e3958d42b8d452c9199d4a221d0d1b15d14655787453dbe77208ced9051705636f696e730442555344000748e0e3958d42b8d452c9199d4a221d0d1b15d14655787453dbe77208ced9051705636f696e730344414900079936836587ca33240d3d3f91844651b16cb07802faf5e34514ed6f78580deb0a0575696e747302553100012120c95db29a67a848940829b3df6119b5e67b788ff0248676e4484c7c6f29c0f5e6c2720700000000006400000000000000503e628d2e00000001");
}

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
