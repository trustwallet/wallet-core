// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../uint256.h"

namespace TW::Seele {

class Transaction {

  public:
    uint256_t type;
    uint256_t accountNonce;
    uint256_t gasPrice;
    uint256_t gasLimit;
    Address from;
    Address to;
    uint256_t amount;
    std::vector<uint8_t> payload;
    uint256_t timestamp;


    // Signature values
    uint256_t v = uint256_t();
    uint256_t r = uint256_t();
    uint256_t s = uint256_t();

    Transaction(uint256_t type,uint256_t accountNonce, uint256_t gasPrice, uint256_t gasLimit,
    Address from,Address to, uint256_t amount, Data payload,uint256_t timestamp)
        : type(std::move(type))
        , accountNonce(std::move(accountNonce))
        , gasPrice(std::move(gasPrice))
        , gasLimit(std::move(gasLimit))
        , from(std::move(from))
        , to(std::move(to))
        , amount(std::move(amount))
        , payload(std::move(payload))
        , timestamp(std::move(timestamp)){}
};

} // namespace TW::Seele
