// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "Data.h"
#include "uint256.h"

namespace TW::Aion {

class Transaction {
  public:

    uint128_t nonce;
    uint128_t gasPrice;
    uint128_t gasLimit;
    Address to;
    uint128_t amount;
    uint128_t timestamp;
    std::vector<uint8_t> payload;

    /// Transaction signature.
    std::vector<uint8_t> signature;

    Transaction(uint128_t nonce, uint128_t gasPrice, uint128_t gasLimit, Address to,
                uint128_t amount, uint128_t timestamp, const Data& payload)
        : nonce(std::move(nonce))
        , gasPrice(std::move(gasPrice))
        , gasLimit(std::move(gasLimit))
        , to(std::move(to))
        , amount(std::move(amount))
        , timestamp(std::move(timestamp))
        , payload(std::move(payload)) {}

  public:
    /// Encodes the transaction.
    Data encode() const noexcept;
};

} // namespace TW::Aion
