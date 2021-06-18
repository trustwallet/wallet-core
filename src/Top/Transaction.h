// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Address.h"
#include "../Data.h"
#include <boost/multiprecision/cpp_int.hpp>

namespace TW::Top {
class Transaction {
  public:
    using uint128_t = boost::multiprecision::uint128_t;

    Address from;
    Address to;
    uint64_t amount;
    uint32_t tx_deposit;
    uint64_t last_tx_nonce;
    std::vector<uint8_t> last_tx_hash;
    std::vector<uint8_t> token;
    std::vector<uint8_t> note;

    /// Transaction signature.
    std::vector<uint8_t> signature;

    Transaction(Address from, Address to, uint64_t amount, uint32_t tx_deposit,
                uint64_t last_tx_nonce, const Data& last_tx_hash, const Data& token, const Data& note)
        : from(std::move(from))
        , to(std::move(to))
        , amount(std::move(amount))
        , tx_deposit(std::move(tx_deposit))
        , last_tx_nonce(std::move(last_tx_nonce))
        , last_tx_hash(std::move(last_tx_hash))
        , token(std::move(token)) 
        , note(std::move(note)) 
        {}

  public:
    /// Encodes the transaction.
    Data encode() const noexcept;
};

} // namespace TW::Top
