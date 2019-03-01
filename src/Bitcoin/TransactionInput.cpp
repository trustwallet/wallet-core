// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionInput.h"

#include "BinaryCoding.h"

using namespace TW::Bitcoin;

void TransactionInput::encode(Data& data) const {
    auto& outpoint = reinterpret_cast<const TW::Bitcoin::OutPoint&>(previousOutput);
    outpoint.encode(data);
    script.encode(data);
    encode32(sequence, data);
}

void TransactionInput::encodeWitness(Data& data) const {
    Bitcoin::writeCompactSize(scriptWitness.size(), data);
    for (auto& item : scriptWitness) {
        Bitcoin::writeCompactSize(item.size(), data);
        std::copy(std::begin(item), std::end(item), std::back_inserter(data));
    }
}
