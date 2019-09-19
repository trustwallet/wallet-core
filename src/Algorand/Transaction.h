// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../Data.h"
#include "../proto/Algorand.pb.h"

namespace TW::Algorand {

class Transaction {
  public:
    Address from;
    Address to;
    uint64_t fee;
    uint64_t amount;
    uint64_t firstRound;
    uint64_t lastRound;
    Data note;
    std::string type;

    Transaction(Address &from, Address &to, uint64_t fee, uint64_t amount, uint64_t firstRound,
                uint64_t lastRound, Data &note, std::string type)
        : from(from)
        , to(to)
        , fee(fee)
        , amount(amount)
        , firstRound(firstRound)
        , lastRound(lastRound)
        , note(note) 
        , type(type) {}

  public:
    Data serialize() const;
    Data serialize(Data &signature) const;
};

} // namespace TW::Algorand
