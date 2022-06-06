// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Script.h"

#include "CommonFunc.h"

#include "../Bech32.h"
#include "../BinaryCoding.h"
#include "../Data.h"

#include <algorithm>
#include <iterator>
#include <set>

using namespace TW;
using namespace TW::Nervos;

Script::Script(const Data& data) {
    std::vector<Data> dataArray = CommonFunc::decodeDataArray(data);
    int i = 0;
    for (auto& element : dataArray[0]) {
        codeHash[i++] = element;
    }
    hashType = (HashType)dataArray[1][0];
    args = dataArray[2];
}

Data Script::hash() const {
    Data data;
    encode(data);
    return TW::Hash::blake2b(data, 32, CommonFunc::getHashPersonalization());
}

bool Script::empty() const {
    for (auto& element : codeHash) {
        if (element != 0) {
            return false;
        }
    }
    return true;
}

void Script::encode(Data& data) const {
    if (empty()) {
        return;
    }
    std::vector<Data> dataArray;
    dataArray.push_back(codeHash);
    dataArray.push_back(Data({(uint8_t)hashType}));
    Data argsData;
    encode32LE((uint32_t)args.size(), argsData);
    std::copy(std::begin(args), std::end(args), std::back_inserter(argsData));
    dataArray.push_back(argsData);
    CommonFunc::encodeDataArray(dataArray, data);
}
