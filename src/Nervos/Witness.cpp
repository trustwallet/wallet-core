// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
