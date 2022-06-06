// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CellOutput.h"

#include "CommonFunc.h"
#include "../BinaryCoding.h"

using namespace TW::Nervos;

void CellOutput::encode(Data& data) const {
    std::vector<Data> dataArray;

    Data capacityData;
    encode64LE(capacity, capacityData);
    dataArray.push_back(capacityData);

    Data lockData;
    lock.encode(lockData);
    dataArray.push_back(lockData);

    Data typeData;
    type.encode(typeData);
    dataArray.push_back(typeData);

    CommonFunc::encodeDataArray(dataArray, data);
}
