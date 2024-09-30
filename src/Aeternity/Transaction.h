// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <Data.h>
#include <string>
#include <uint256.h>

namespace TW::Aeternity {

class Transaction {

  public:
    std::string sender_id;

    std::string recipient_id;

    uint256_t amount;

    uint256_t fee;

    std::string payload;

    /// the block time that tx live on the mempool, you can use 0 by default, or >latest block
    uint64_t ttl;

    uint64_t nonce;

    Transaction(
        std::string& sender_id,
        std::string& recipientId,
        uint256_t amount,
        uint256_t fee,
        std::string& payload,
        uint64_t ttl,
        uint64_t nonce
    )
        : sender_id(sender_id)
        , recipient_id(recipientId)
        , amount(std::move(amount))
        , fee(std::move(fee))
        , payload(payload)
        , ttl(ttl)
        , nonce(nonce){};

    Data encode();

    //// buildIDTag assemble an id() object
    //// see https://github.com/aeternity/protocol/blob/epoch-v0.22.0/serializations.md#the-id-type
    static Data buildTag(const std::string& address);
};

} // namespace TW::Aeternity
