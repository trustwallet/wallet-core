// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/BinaryCoding.h"
#include "Tezos/OperationList.h"
#include "Tezos/Signer.h"
#include "Tezos/Transaction.h"
#include "PrivateKey.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Tezos;

TEST(TezosSigner, SignString) {
    std::string bytesToSign = "ffaa";
    std::string expectedSignature = "eaab7f4066217b072b79609a9f76cdfadd93f8dde41763887e131c02324f18c8e41b1009e334baf87f9d2e917bf4c0e73165622e5522409a0c5817234a48cc02";
    std::string expectedSignedBytes =  bytesToSign + expectedSignature;

    auto key = PrivateKey(parse_hex("0x2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f"));    
    auto signedBytes = Signer().signHexString(key, bytesToSign);

    ASSERT_EQ(signedBytes, expectedSignedBytes);
}

TEST(TezosSigner, SignOperationList) {
    auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    auto op_list = OperationList(branch);
    auto tx1 = Transaction(
        Address("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"),
        1272,
        30738,
        10100,
        257,
        1,
        parsePublicKey("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A"),
        operationtype::REVEAL
    );
    op_list.add_operation(tx1);
    auto tx2 = Transaction(
        Address("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"),
        1272,
        30739,
        10100,
        257,
        1,
        Address("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW"),
        operationtype::TRANSACTION
    );
    op_list.add_operation(tx2);

    std::string expectedForgedBytesToSign = op_list.forge();
    std::string expectedSignature = "d924cb3e56c4b9f55e50735e461899a2f616a26bfb0aa05d0b356b66f517b023df330ad3621f0bf39d518131a1becd6a7b2e226ed291483af3682535d1f4530f";
    std::string expectedSignedBytes = expectedForgedBytesToSign + expectedSignature;

    auto key = PrivateKey(parse_hex("0x2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f"));
    auto signedBytes = Signer().signOperationList(key, op_list);

    ASSERT_EQ(signedBytes, expectedSignedBytes);
}
