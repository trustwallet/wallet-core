// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "CellOutput.h"
#include "Serialization.h"

namespace TW::Nervos {

void CellOutput::encode(Data& data) const {
    Data capacityData;
    Data lockData;
    Data typeData;
    encode64LE(capacity, capacityData);
    lock.encode(lockData);
    type.encode(typeData);
    Serialization::encodeDataArray(std::vector<Data>{capacityData, lockData, typeData}, data);
}

nlohmann::json CellOutput::json() const {
    return nlohmann::json{{"capacity", Serialization::numberToHex(capacity)},
                          {"lock", lock.json()},
                          {"type", type.json()}};
}

} // namespace TW::Nervos
