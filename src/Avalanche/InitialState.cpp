// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "InitialState.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

void InitialState::encode(Data& data) const {
    encode32LE(FxID, data);
    encode32LE(Outputs.size(), data);
    for (auto output : Outputs) {
        output.encode(data);
    }
}

bool InitialState::operator<(const InitialState& other) {
    Data thisData;
    Data otherData;
    encode(thisData);
    other.encode(otherData);
    return std::lexicographical_compare(thisData.begin(), thisData.end(), otherData.begin(), otherData.end());
}