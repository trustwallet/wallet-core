// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "BinaryCoding.h"
#include "Data.h"
#include "PublicKey.h"
#include "SS58Address.h"
#include "uint256.h"

#include <cmath>
#include <algorithm>
#include <bitset>

using CompactInteger = TW::uint256_t;

namespace TW::Polkadot {

static constexpr size_t kMinUint16 = (1ul << 6u);
static constexpr size_t kMinUint32 = (1ul << 14u);
static constexpr size_t kMinBigInteger = (1ul << 30u);
// max uint8
static constexpr byte kMaxByte = 255;

inline size_t countBytes(CompactInteger value) {
    if (0 == value) {
        return 1;
    }

    size_t size = 0;
    while (value > 0) {
        ++size;
        value >>= 8;
    }

    return size;
}

inline Data encodeCallIndex(int32_t moduleIndex, int32_t methodIndex) {
    if (moduleIndex > kMaxByte) {
        throw std::invalid_argument("module index too large");
    }
    if (methodIndex > kMaxByte) {
        throw std::invalid_argument("method index too large");
    }

    return Data{static_cast<byte>(moduleIndex), static_cast<byte>(methodIndex)};
}

inline Data encodeCompact(CompactInteger value) {
    auto data = Data{};

    if (value < kMinUint16) {
        auto v = value.convert_to<uint8_t>() << 2u;
        data.push_back(static_cast<uint8_t>(v));
        return data;
    } else if (value < kMinUint32) {
        auto v = (value.convert_to<uint16_t>() << 2u);
        v += 0x01; // set 0b01 flag
        auto minor_byte = static_cast<uint8_t>(v & 0xffu);
        data.push_back(minor_byte);
        v >>= 8u;
        auto major_byte = static_cast<uint8_t>(v & 0xffu);
        data.push_back(major_byte);
        return data;
    } else if (value < kMinBigInteger) {
        uint32_t v = (value.convert_to<uint32_t>() << 2u);
        v += 0x02; // set 0b10 flag
        encode32LE(v, data);
        return data;
    }

    auto length = countBytes(value);
    if (length > 67) {
        // too big
        return data;
    }
    uint8_t header = (static_cast<uint8_t>(length) - 4) * 4;
    header += 0x03; // set 0b11 flag;
    data.push_back(header);

    auto v = CompactInteger{value};
    for (size_t i = 0; i < length; ++i) {
        data.push_back(static_cast<uint8_t>(v & 0xff)); // push back least significant byte
        v >>= 8;
    }
    return data;
}

// append length prefix
inline void encodeLengthPrefix(Data& data) {
    size_t len = data.size();
    auto prefix = encodeCompact(len);
    data.insert(data.begin(), prefix.begin(), prefix.end());
}

inline Data encodeBool(bool value) {
    return Data{uint8_t(value ? 0x01 : 0x00)};
}

inline Data encodeVector(const std::vector<Data>& vec) {
    auto data = encodeCompact(vec.size());
    for (auto v : vec) {
        append(data, v);
    }
    return data;
}

inline Data encodeAccountId(const Data& bytes, bool raw) {
    auto data = Data{};
    if (!raw) {
        // MultiAddress::AccountId
        // https://github.com/paritytech/substrate/blob/master/primitives/runtime/src/multiaddress.rs#L28
        append(data, 0x00);
    }
    append(data, bytes);
    return data;
}

inline Data encodeAccountIds(const std::vector<SS58Address>& addresses, bool raw) {
    std::vector<Data> vec;
    for (auto addr : addresses) {
        vec.push_back(encodeAccountId(addr.keyBytes(), raw));
    }
    return encodeVector(vec);
}

inline Data encodeEra(const uint64_t block, const uint64_t period) {
    // MortalEra(phase, period)
    // See decodeMortalObject at https://github.com/polkadot-js/api/blob/master/packages/types/src/extrinsic/ExtrinsicEra.ts#L87
    // See toU8a at https://github.com/polkadot-js/api/blob/master/packages/types/src/extrinsic/ExtrinsicEra.ts#L167
    uint64_t calPeriod = uint64_t(pow(2, ceil(log2(double(period)))));
    calPeriod = std::min(std::max(calPeriod, uint64_t(4)), uint64_t(1) << 16);
    uint64_t phase = block % calPeriod;
    uint64_t quantizeFactor = std::max(calPeriod >> uint64_t(12), uint64_t(1));
    uint64_t quantizedPhase = phase / quantizeFactor * quantizeFactor;

    auto bitset = std::bitset<64>(calPeriod);
    int trailingZeros = 0;
    for (int i = 0; i < 64 - 1; i++) {
        if (bitset[i] == 0) {
            trailingZeros += 1;
        } else {
            break;
        }
    }
    auto encoded = std::min(15, std::max(1, trailingZeros - 1)) + (((quantizedPhase / quantizeFactor) << 4));
    return Data{byte(encoded & 0xff), byte(encoded >> 8)};
}

} // namespace TW::Polkadot
