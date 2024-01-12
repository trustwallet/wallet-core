// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "BinaryCoding.h"

#include <cassert>
#include <tuple>

namespace TW {

using namespace std;

void encode64LE(uint64_t val, vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
    data.push_back(static_cast<uint8_t>((val >> 48)));
    data.push_back(static_cast<uint8_t>((val >> 56)));
}

uint64_t decode64LE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[0])
        | (static_cast<uint64_t>(src[1]) << 8)
        | (static_cast<uint64_t>(src[2]) << 16)
        | (static_cast<uint64_t>(src[3]) << 24)
        | (static_cast<uint64_t>(src[4]) << 32)
        | (static_cast<uint64_t>(src[5]) << 40)
        | (static_cast<uint64_t>(src[6]) << 48)
        | (static_cast<uint64_t>(src[7]) << 56);
    // clang-format on
}

uint8_t varIntSize(uint64_t value) {
    // The value is small enough to be represented by itself.
    if (value < 0xfd) {
        return 1;
    }

    // Discriminant 1 byte plus 2 bytes for the uint16.
    if (value <= UINT16_MAX) {
        return 1 + 2;
    }

    // Discriminant 1 byte plus 4 bytes for the uint32.
    if (value <= UINT32_MAX) {
        return 1 + 4;
    }

    // Discriminant 1 byte plus 8 bytes for the uint64.
    return 1 + 8;
}

uint8_t encodeVarInt(uint64_t size, vector<uint8_t>& data) {
    if (size < 0xfd) {
        data.push_back(static_cast<uint8_t>(size));
        return 1;
    }

    if (size <= UINT16_MAX) {
        data.push_back(0xfd);
        encode16LE((uint16_t)size, data);
        return 3;
    }

    if (size <= UINT32_MAX) {
        data.push_back(0xfe);
        encode32LE((uint32_t)size, data);
        return 5;
    }

    data.push_back(0xff);
    encode64LE((uint64_t)size, data);
    return 9;
}

tuple<bool, uint64_t> decodeVarInt(const Data& in, size_t& indexInOut) {
    if (in.size() < indexInOut + 1) {
        // too short
        return make_tuple(false, 0);
    }
    // extra bytes except first
    uint8_t size = 0;
    const auto firstByte = in[indexInOut];
    switch (firstByte) {
        case 0xfd: size = 2; break;
        case 0xfe: size = 4; break;
        case 0xff: size = 8; break;
        default: size = 0; break; // one-byte case is one byte, with not discriminator
    }
    ++indexInOut;
    assert(size == 0 || size == 2 || size == 4 || size == 8);
    if (in.size() < indexInOut + size) {
        // too short
        return make_tuple(false, 0);
    }

    uint64_t number = 0;
    switch (size) {
        default:
        case 0: number = firstByte; break;
        case 2: number = decode16LE(in.data() + indexInOut); break;
        case 4: number = decode32LE(in.data() + indexInOut); break;
        case 8: number = decode64LE(in.data() + indexInOut); break;
    }
    indexInOut += size;
    return make_tuple(true, number);
}

void encode64BE(uint64_t val, vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 56)));
    data.push_back(static_cast<uint8_t>((val >> 48)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>(val));
}

uint64_t decode64BE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[7])
        | (static_cast<uint64_t>(src[6]) << 8)
        | (static_cast<uint64_t>(src[5]) << 16)
        | (static_cast<uint64_t>(src[4]) << 24)
        | (static_cast<uint64_t>(src[3]) << 32)
        | (static_cast<uint64_t>(src[2]) << 40)
        | (static_cast<uint64_t>(src[1]) << 48)
        | (static_cast<uint64_t>(src[0]) << 56);
    // clang-format on
}

void encodeString(const string& str, vector<uint8_t>& data) {
    size_t size = str.size();
    encodeVarInt(size, data);
    data.insert(data.end(), str.data(), str.data() + size);
}

/// Decodes an ASCII string prefixed by its length (varInt) 
tuple<bool, string>  decodeString(const Data& in, size_t& indexInOut) {
    const auto lenTup = decodeVarInt(in, indexInOut);
    if (!get<0>(lenTup)) { return make_tuple(false, ""); }
    const auto len = get<1>(lenTup);
    // read bytes into string
    if (in.size() < indexInOut + len) { return make_tuple(false, ""); }
    string result(in.data() + indexInOut, in.data() + indexInOut + len);
    indexInOut += len;
    return make_tuple(true, result);
}

} // namespace TW
