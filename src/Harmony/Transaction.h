// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstdint>
#include <stdint.h>
#include <vector>

#include "Address.h"
#include "../uint256.h"

namespace TW::Harmony {

class Transaction {
  public:
    uint256_t nonce;
    uint256_t gasPrice;
    uint256_t gasLimit;
    uint256_t fromShardID;
    uint256_t toShardID;
    Address to;
    uint256_t amount;
    std::vector<uint8_t> payload;

    // Signature values
    uint256_t v = uint256_t();
    uint256_t r = uint256_t();
    uint256_t s = uint256_t();

    Transaction() = default;

    Transaction(uint256_t nonce, uint256_t gasPrice, uint256_t gasLimit, uint256_t fromShardID,
                uint256_t toShardID, Address to, uint256_t amount, const Data& payload)
        : nonce(std::move(nonce))
        , gasPrice(std::move(gasPrice))
        , gasLimit(std::move(gasLimit))
        , fromShardID(std::move(fromShardID))
        , toShardID(std::move(toShardID))
        , to(std::move(to))
        , amount(std::move(amount))
        , payload(std::move(payload)) {}
};

} // namespace TW::Harmony
