// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
        std::string &sender_id,
        std::string &recipientId,
        uint256_t amount,
        uint256_t fee,
        std::string &payload,
        uint64_t ttl,
        uint64_t nonce
    )
        : sender_id(sender_id)
        , recipient_id(recipientId)
        , amount(amount)
        , fee(fee)
        , payload(payload)
        , ttl(ttl)
        , nonce(nonce){};

    Data encode();

    static Data buildTag(const std::string &address);

    static Data encodeSafeZero(uint256_t value);


};

} // namespace TW::Aeternity
