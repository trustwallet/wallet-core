// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "BaseTransaction.h"
#include "Data.h"
#include "../proto/Algorand.pb.h"

namespace TW::Algorand {

class AssetTransfer: public BaseTransaction {
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

    AssetTransfer(Address &from, Address &to, uint64_t fee, uint64_t amount, uint64_t assetId, uint64_t firstRound,
                     uint64_t lastRound, Data& note, std::string type, std::string& genesisId, Data& genesisHash)
        : from(from), to(to)
        , fee(fee), amount(amount)
        , assetId(assetId), firstRound(firstRound)
        , lastRound(lastRound), note(note)
        , type(std::move(type)), genesisId(genesisId)
        , genesisHash(genesisHash) {}

  public:
    Data serialize() const override;
};

} // namespace TW::Algorand
