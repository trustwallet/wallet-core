// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../Data.h"

#include <boost/multiprecision/cpp_int.hpp>

namespace TW {
namespace Aion {

class Transaction {
  public:
    typedef boost::multiprecision::uint128_t uint128_t;

    uint128_t nonce;
    uint128_t gasPrice;
    uint128_t gasLimit;
    Address to;
    uint128_t amount;
    std::vector<uint8_t> payload;

    /// Transaction signature.
    std::vector<uint8_t> signature;

    Transaction() = default;
    Transaction(uint128_t nonce, uint128_t gasPrice, uint128_t gasLimit, const Address& to,
                uint128_t amount, const std::vector<uint8_t>& payload)
        : nonce(nonce)
        , gasPrice(gasPrice)
        , gasLimit(gasLimit)
        , to(to)
        , amount(amount)
        , payload(payload) {}

  public:
    /// Encodes the transaction.
    Data encode() const noexcept;
};

} // namespace Aion
} // namespace TW
