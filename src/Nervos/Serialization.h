// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../BinaryCoding.h"
#include "Data.h"
#include "../HexCoding.h"
#include "../uint256.h"

#include <numeric>
#include <string>

namespace TW::Nervos {

struct Serialization {
    static void encodeDataArray(const std::vector<Data>& dataArray, Data& data) {
        uint32_t dataLength = std::accumulate(dataArray.begin(), dataArray.end(), uint32_t(0),
                                              [](const uint32_t total, const Data& element) {
                                                  return total + uint32_t(element.size());
                                              });
        uint32_t headerLength = 4 + 4 * uint32_t(dataArray.size());
        uint32_t fullLength = headerLength + dataLength;
        encode32LE(fullLength, data);
        std::accumulate(dataArray.begin(), dataArray.end(), headerLength,
                        [&data](const uint32_t offset, const Data& element) {
                            encode32LE(offset, data);
                            return offset + uint32_t(element.size());
                        });
        for (auto&& element : dataArray) {
            data.insert(data.end(), element.begin(), element.end());
        }
    }

    static Data encodeUint256(uint256_t number, byte minLen = 0) {
        auto data = store(number, minLen);
        std::reverse(data.begin(), data.end());
        return data;
    }

    static uint256_t decodeUint256(const Data& data) {
        auto data1 = Data(data);
        std::reverse(data1.begin(), data1.end());
        return load(data1);
    }

    static std::string numberToHex(uint64_t number) {
        auto str = hex(number);
        str.erase(0, str.find_first_not_of('0'));
        if (str.length() == 0) {
            return "0x0";
        } else {
            return str.insert(0, "0x");
        }
    }
};
} // namespace TW::Nervos
