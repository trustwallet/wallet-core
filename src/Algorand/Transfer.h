// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <utility>

#include "Address.h"
#include "BaseTransaction.h"
#include "Data.h"
#include "../proto/Algorand.pb.h"

namespace TW::Algorand {

class Transfer : public BaseTransaction {
  public:
    Address from;
    Address to;
    uint64_t fee;
    uint64_t amount;
    uint64_t firstRound;
    uint64_t lastRound;
    Data note;
    std::string type;

    std::string genesisId;
    Data genesisHash;

    Transfer(Address &from, Address &to, uint64_t fee, uint64_t amount, uint64_t firstRound,
                uint64_t lastRound, Data& note, std::string type, std::string& genesisIdg, Data& genesisHash)
        : from(from) , to(to)
        , fee(fee), amount(amount)
        , firstRound(firstRound), lastRound(lastRound)
        , note(note), type(std::move(type))
        , genesisId(genesisIdg), genesisHash(genesisHash) {}

  public:
    Data serialize() const override;
};

} // namespace TW::Algorand
