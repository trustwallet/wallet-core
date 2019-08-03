// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../uint256.h"

namespace TW::Nebulas {

class Transaction {
  public:
    Address from;
    uint256_t nonce;
    uint256_t gasPrice;
    uint256_t gasLimit;
    Address to;
    uint256_t amount;
    uint256_t timestamp;
    std::vector<uint8_t> payload;

    // Signature values
    Data signature;
    uint32_t algorithm;

    Transaction(Address from, uint256_t nonce, uint256_t gasPrice, uint256_t gasLimit, Address to, uint256_t amount, uint256_t timestamp, Data payload)
        : from(std::move(from))
        , nonce(std::move(nonce))
        , gasPrice(std::move(gasPrice))
        , gasLimit(std::move(gasLimit))
        , to(std::move(to))
        , amount(std::move(amount))
        , timestamp(std::move(timestamp))
        , payload(std::move(payload)){}
};

} // namespace TW::Nebulas
