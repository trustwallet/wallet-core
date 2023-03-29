// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "Data.h"

#include <boost/multiprecision/cpp_int.hpp>

namespace TW::Aion {

class Transaction {
  public:
    using uint128_t = boost::multiprecision::uint128_t;

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
