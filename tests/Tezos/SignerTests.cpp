// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/BinaryCoding.h"
#include "Tezos/OperationList.h"
#include "Tezos/Signer.h"
#include "PrivateKey.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Tezos;

TEST(TezosSigner, SignString) {
    Data bytesToSign = parse_hex("ffaa");
    Data expectedSignature = parse_hex("eaab7f4066217b072b79609a9f76cdfadd93f8dde41763887e131c02324f18c8e41b1009e334baf87f9d2e917bf4c0e73165622e5522409a0c5817234a48cc02");
    Data expected = Data();
    append(expected, bytesToSign);
    append(expected, expectedSignature);

    auto key = PrivateKey(parse_hex("0x2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f"));    
    auto signedBytes = Signer().signData(key, bytesToSign);

    ASSERT_EQ(signedBytes, expected);
}

TEST(TezosSigner, SignOperationList) {
    auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    auto op_list = TW::Tezos::OperationList(branch);
    
    auto transactionOperationData = new TW::Tezos::Proto::TransactionOperationData();
    transactionOperationData -> set_amount(1);
    transactionOperationData -> set_destination("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");

    auto transactionOperation = TW::Tezos::Proto::Operation();
    transactionOperation.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    transactionOperation.set_fee(1272);
    transactionOperation.set_counter(30739);
    transactionOperation.set_gas_limit(10100);
    transactionOperation.set_storage_limit(257);
    transactionOperation.set_kind(TW::Tezos::Proto::Operation::TRANSACTION);
    transactionOperation.set_allocated_transaction_operation_data(transactionOperationData);
    
    PublicKey publicKey = parsePublicKey("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A");
    
    auto revealOperationData = new TW::Tezos::Proto::RevealOperationData();
    revealOperationData -> set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto revealOperation = TW::Tezos::Proto::Operation();
    revealOperation.set_source("tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW");
    revealOperation.set_fee(1272);
    revealOperation.set_counter(30738);
    revealOperation.set_gas_limit(10100);
    revealOperation.set_storage_limit(257);
    revealOperation.set_kind(TW::Tezos::Proto::Operation::REVEAL);
    revealOperation.set_allocated_reveal_operation_data(revealOperationData);
    
    op_list.addOperation(revealOperation);
    op_list.addOperation(transactionOperation);

    std::string expectedForgedBytesToSign = hex(op_list.forge());
    std::string expectedSignature = "d924cb3e56c4b9f55e50735e461899a2f616a26bfb0aa05d0b356b66f517b023df330ad3621f0bf39d518131a1becd6a7b2e226ed291483af3682535d1f4530f";
    std::string expectedSignedBytes = expectedForgedBytesToSign + expectedSignature;
    auto key = PrivateKey(parse_hex("0x2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f"));
    auto signedBytes = Signer().signOperationList(key, op_list);

    ASSERT_EQ(signedBytes, parse_hex(expectedSignedBytes));
}
