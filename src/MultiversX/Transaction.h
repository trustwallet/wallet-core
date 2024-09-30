// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <cstdint>
#include <string>

namespace TW::MultiversX {

enum TransactionOptions : uint32_t {
    Default = 0,
    // Not applicable for applications based on TW Core (as of April 2023).
    HashSign = 1,
    // Whether the transaction is guarded (using a guardian account).
    // Generally speaking, applications can ignore this option (though some can choose to implement guarded transactions).
    Guarded = 2
};

class Transaction {
public:
    uint64_t nonce;
    std::string sender;
    std::string senderUsername;
    std::string receiver;
    std::string receiverUsername;
    std::string guardian;
    std::string value;
    std::string data;
    uint64_t gasPrice;
    uint64_t gasLimit;
    std::string chainID;
    uint32_t version;
    TransactionOptions options;

    Transaction();

    bool hasGuardian() const;
};

} // namespace TW::MultiversX
