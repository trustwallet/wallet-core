//
// Created by Andrew Vasiliev on 26.07.2021.
//


#pragma once

#include "Address.h"
#include "../Data.h"
#include "../proto/Algorand.pb.h"

namespace TW::Algorand {

class AssetTransaction {
  public:
    Address from;
    Address to;
    uint64_t fee;
    uint64_t amount;
    uint64_t assetId;
    uint64_t firstRound;
    uint64_t lastRound;
    Data note;
    std::string type;

    std::string genesisId;
    Data genesisHash;

    AssetTransaction(Address &from, Address &to, uint64_t fee, uint64_t amount, uint64_t assetId, uint64_t firstRound,
                     uint64_t lastRound, Data& note, std::string type, std::string& genesisId, Data& genesisHash)
        : from(from), to(to)
        , fee(fee), amount(amount)
        , assetId(assetId), firstRound(firstRound)
        , lastRound(lastRound), note(note)
        , genesisId(genesisId), genesisHash(genesisHash)
        , type(type) {}

  public:
    Data serialize() const;
    Data serialize(Data& signature) const;
};

} // namespace TW::Algorand
