// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Witness.h"
#include "Serialization.h"

namespace TW::Nervos {

void Witness::encode(Data& data) const {
    if ((lock.empty()) && (inputType.empty()) && (outputType.empty())) {
        return;
    }
    std::vector<Data> dataArray;
    dataArray.reserve(3);
    for (auto&& data1 : std::vector<Data>({lock, inputType, outputType})) {
        Data data2;
        if (!data1.empty()) {
            encode32LE(uint32_t(data1.size()), data2);
            data2.insert(data2.end(), data1.begin(), data1.end());
        }
        dataArray.emplace_back(data2);
    }
    Serialization::encodeDataArray(dataArray, data);
}

} // namespace TW::Nervos
