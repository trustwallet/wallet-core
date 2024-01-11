// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TransactionInput.h"

#include "../BinaryCoding.h"

namespace TW::Bitcoin {

void TransactionInput::encode(Data& data) const {
    auto& outpoint = reinterpret_cast<const TW::Bitcoin::OutPoint&>(previousOutput);
    outpoint.encode(data);
    script.encode(data);
    encode32LE(sequence, data);
}

void TransactionInput::encodeWitness(Data& data) const {
    encodeVarInt(scriptWitness.size(), data);
    for (auto& item : scriptWitness) {
        encodeVarInt(item.size(), data);
        std::copy(std::begin(item), std::end(item), std::back_inserter(data));
    }
}

} // namespace TW::Bitcoin
