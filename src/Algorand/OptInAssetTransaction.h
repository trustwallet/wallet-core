//
// Created by Andrew Vasiliev on 30.07.2021.
//

#include "Address.h"
#include "../Data.h"
#include "../proto/Algorand.pb.h"

namespace TW::Algorand {

class OptInAssetTransaction {
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
        , type(type), genesisId(genesisId)
        , genesisHash(genesisHash) {}

  public:
    Data serialize() const;
    Data serialize(Data& signature) const;
};

} // namespace TW::Algorand
