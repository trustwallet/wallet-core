// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"

namespace TW::Semux {

class Transaction {
  public:
    // Network id
    uint8_t network;
    // Transaction type id
    uint8_t transactionType;
    // Recipient address
    Address to;
    // Transaction value
    uint64_t value;
    // Transaction fee
    uint64_t fee;
    // Transaction nonce
    uint64_t nonce;
    // Transaction timestamp
    uint64_t timestamp;
    // Transaction data
    Data data;

    // Sender signature
    std::array<uint8_t, 96> signature;

    Transaction(Address to, uint64_t value, uint64_t fee, uint64_t nonce, uint64_t timestamp)
        : network(static_cast<uint8_t>(0))
        , transactionType(static_cast<uint8_t>(1))
        , to(to)
        , value(value)
        , fee(fee)
        , nonce(nonce)
        , timestamp(timestamp)
        , data(Data()){};

  public:
    std::vector<uint8_t> serialize() const;
    std::vector<uint8_t> getPreImage() const;
    std::vector<uint8_t> getHash() const;

  private:
    void writeBytes(const Data &bytes, Data &buffer) const;
};

} // namespace TW::Semux

/// Wrapper for C interface.
struct TWSemuxTransaction {
    TW::Semux::Transaction impl;
};
