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

class OptInAssetTransaction: public BaseTransaction {
  public:
    Address address;
    uint64_t fee;
    uint64_t assetId;
    uint64_t firstRound;
    uint64_t lastRound;
    Data note;
    std::string type;

    std::string genesisId;
    Data genesisHash;

    OptInAssetTransaction(Address &address, uint64_t fee, uint64_t assetId, uint64_t firstRound,
                          uint64_t lastRound, Data& note, std::string type, std::string& genesisId, Data& genesisHash)
        : address(address), fee(fee)
        , assetId(assetId), firstRound(firstRound)
        , lastRound(lastRound), note(note)
        , type(std::move(type)), genesisId(genesisId)
        , genesisHash(genesisHash) {}

  public:
    Data serialize() const override;
};

} // namespace TW::Algorand
