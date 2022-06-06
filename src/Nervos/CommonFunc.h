// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "CellDep.h"
#include "OutPoint.h"
#include "../BinaryCoding.h"
#include "../Data.h"
#include "../HexCoding.h"

namespace TW::Nervos {

enum AddressType {
    FullVersion = 0,  // full version identifies the hash_type
    HashIdx = 1,      // short version for locks with popular codehash, deprecated
    DataCodeHash = 2, // full version with hash type 'Data', deprecated
    TypeCodeHash = 4, // full version with hash type 'Type', deprecated
};

enum HashType { Data0 = 0, Type1 = 1, Data1 = 2 };

class CommonFunc {
  public:
    static Data getHashPersonalization() {
        return Data(
            {'c', 'k', 'b', '-', 'd', 'e', 'f', 'a', 'u', 'l', 't', '-', 'h', 'a', 's', 'h'});
    }

    static Data getSecp256k1CodeHash() {
        return parse_hex("9bd7e06f3ecf4be0f2fcd2188b23f1b9fcc88e5d4b65a8637b17723bbda3cce8");
    }

    static CellDep getSecp256k1CellDep() {
        return CellDep(
            OutPoint(parse_hex("71a7ba8fc96349fea0ed3a5c47992e3b4084b031a42264a018e0072e8172e46c"),
                     0),
            DepType_DepGroup);
    }

    static void encodeDataArray(std::vector<Data>& dataArray, Data& data) {
        uint32_t dataLength = 0;
        for (auto& element : dataArray) {
            dataLength += (uint32_t)element.size();
        }
        uint32_t headerLength = 4 + 4 * (uint32_t)dataArray.size();
        uint32_t fullLength = headerLength + dataLength;
        encode32LE(fullLength, data);
        uint32_t offset = headerLength;
        for (auto& element : dataArray) {
            encode32LE(offset, data);
            offset += (uint32_t)element.size();
        }
        for (auto& element : dataArray) {
            std::copy(std::begin(element), std::end(element), std::back_inserter(data));
        }
    }

    static std::vector<Data> decodeDataArray(const Data& data) {
        std::vector<Data> dataArray;
        uint32_t headerLength = decode32LE(data.data() + 4);
        uint32_t numElements = (headerLength - 4) / 4;
        uint32_t previousOffset = headerLength;
        for (int i = 0; i < numElements; i++) {
            uint32_t nextOffset = decode32LE(data.data() + 8 + 4 * i);
            dataArray.push_back(Data(data.begin() + previousOffset, data.begin() + nextOffset));
            previousOffset = nextOffset;
        }
        return dataArray;
    }
};
} // namespace TW::Nervos
