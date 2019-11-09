// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../Data.h"

namespace TW::ARK {

class Transaction {
  public:
    byte type;
    uint64_t amount;
    uint64_t fee;
    uint32_t timestamp;
    Address to;
    Data publicKey;

    /// Transaction signature.
    Data signature;

    Transaction(byte type, uint64_t amount, uint64_t fee, uint32_t timestamp, Address to,
                Data publicKey)
        : type(type)
        , amount(amount)
        , fee(fee)
        , timestamp(timestamp)
        , to(to)
        , publicKey(publicKey) {}

  public:
    /// Encodes the transaction.
    Data encoded() const;

    /// Encodes the transaction as json string.
    std::string encodedJson() const;
};

} // namespace TW::ARK
