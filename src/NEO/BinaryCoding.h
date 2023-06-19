// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "OpCode.h"
#include "uint256.h"
#include "../BinaryCoding.h"

namespace TW::NEO {

// Append a uint256_t value as a little-endian byte array into the provided buffer, and limit
// the array size by digit/8.
// Noted: No padding with it.
inline void encode256LE(Data& data, const uint256_t& value) {
    Data bytes = store(value);
    data.insert(data.end(), bytes.rbegin(), bytes.rend());
    if (data.back() >= 128) {
        data.push_back(0x00);
    }
}

inline void encodeBytes(Data& data, const Data& value) {
    if (value.size() <= (size_t)PUSHBYTES75) {
        data.push_back((byte)value.size());
        data.insert(data.end(), value.begin(), value.end());
    } else if (value.size() < 0x100) {
        data.push_back(PUSHDATA1);
        data.push_back((byte)value.size());
        data.insert(data.end(), value.begin(), value.end());
    } else if (value.size() < 0x10000) {
        data.push_back(PUSHDATA2);
        encode16LE((uint16_t)value.size(), data);
        data.insert(data.end(), value.begin(), value.end());
    } else {
        data.push_back(PUSHDATA4);
        encode32LE((uint32_t)value.size(), data);
        data.insert(data.end(), value.begin(), value.end());
    }
}

} // namespace TW::NEO
