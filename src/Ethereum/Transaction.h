// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include <boost/multiprecision/cpp_int.hpp>

namespace TW {
namespace Ethereum {

class Transaction {
public:
    typedef boost::multiprecision::uint256_t uint256_t;

    uint256_t nonce;
    uint256_t gasPrice;
    uint256_t gasLimit;
    Address to;
    uint256_t amount;
    std::vector<uint8_t> payload;

    // Signature values
    uint256_t v = uint256_t();
    uint256_t r = uint256_t();
    uint256_t s = uint256_t();

    Transaction() = default;
    Transaction(uint256_t nonce, uint256_t gasPrice, uint256_t gasLimit, Address to, uint256_t amount, const std::vector<uint8_t>& payload)
        : nonce(nonce)
        , gasPrice(gasPrice)
        , gasLimit(gasLimit)
        , to(to)
        , amount(amount)
        , payload(payload)
        {}
};

}} // namespace
