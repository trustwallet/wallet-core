// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Witness.h"
#include "CommonFunc.h"
#include "../BinaryCoding.h"

using namespace TW;
using namespace TW::Nervos;

void Witness::encode(Data& data) const {
    std::vector<Data> dataArray;

    for (auto& data1 : std::vector<Data>({lock, inputType, outputType})) {
        Data data2;
        if (data1.size() > 0) {
            encode32LE((uint32_t)data1.size(), data2);
            std::copy(std::begin(data1), std::end(data1), std::back_inserter(data2));
        }
        dataArray.push_back(data2);
    }

    CommonFunc::encodeDataArray(dataArray, data);
}
