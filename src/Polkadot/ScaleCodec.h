// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../BinaryCoding.h"
#include "../Data.h"
#include "../SS58Address.h"
#include "../PublicKey.h"
#include <boost/multiprecision/cpp_int.hpp>

/// Reference https://github.com/soramitsu/kagome/blob/master/core/scale/scale_encoder_stream.cpp
using CompactInteger = boost::multiprecision::cpp_int;

namespace TW::Polkadot {

static constexpr size_t kMinUint16 = (1ul << 6u);
static constexpr size_t kMinUint32 = (1ul << 14u);
static constexpr size_t kMinBigInteger = (1ul << 30u);

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

inline Data encodeCompact(CompactInteger value) {
    auto data = Data{};

    if (value < kMinUint16) {
        data.push_back(value.convert_to<uint8_t>() << 2u);
        return data;
    } else if (value < kMinUint32) {
        auto v = (value.convert_to<uint16_t>() << 2u);
        v += 0b01; // set 0b01 flag
        auto minor_byte = static_cast<uint8_t>(v & 0xffu);
        data.push_back(minor_byte);
        v >>= 8u;
        auto major_byte = static_cast<uint8_t>(v & 0xffu);
        data.push_back(major_byte);
        return data;
    } else if (value < kMinBigInteger) {
        uint32_t v = (value.convert_to<uint32_t>() << 2u);
        v += 0b10; // set 0b10 flag
        encode32LE(v, data);
        return data;
    }

    auto length = countBytes(value);
    if (length > 67) {
        // too big
        return data;
    }
    uint8_t header = (length - 4) * 4;
    header += 0b11; // set 0b11 flag;
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

inline Data encodeAddress(const SS58Address& address) {
    auto data = Data{0xff};
    append(data, Data(address.bytes.begin() + 1, address.bytes.end()));
    return data;
}

inline Data encodeAddress(const PublicKey& key) {
    auto data = Data{0xff};
    append(data, Data(key.bytes.begin(), key.bytes.end()));
    return data;
}

inline Data encodeBool(bool value) {
    return Data {uint8_t(value ? 0x01: 0x00)};
}

inline Data encodeVector(std::vector<Data> vec) {
    return Data{};
}

} // namespace TW::Polkadot
