// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../BinaryCoding.h"

namespace TW::Algorand {

#pragma GCC diagnostic ignored "-Wtautological-constant-out-of-range-compare"
#pragma GCC diagnostic ignored "-Wunused-function"

static inline void encodeString(std::string string, Data& data) {
    // encode string header
    auto bytes = Data(string.begin(), string.end());
    if (bytes.size() < 0x20) {
        // fixstr
        data.push_back(static_cast<uint8_t>(0xa0 + bytes.size()));
    } else if (bytes.size() < 0x100) {
        // str 8
        data.push_back(static_cast<uint8_t>(0xd9));
        data.push_back(static_cast<uint8_t>(bytes.size()));
    } else if (bytes.size() < 0x10000) {
        // str 16
        data.push_back(static_cast<uint8_t>(0xda));
        encode16BE(static_cast<uint16_t>(bytes.size()), data);
    } else if (bytes.size() < 0x100000000) { // depending on size_t size on platform, may be always true 
        // str 32
        data.push_back(static_cast<uint8_t>(0xdb));
        encode32BE(static_cast<uint32_t>(bytes.size()), data);
    } else {
        // too long string
        return;
    }
    append(data, bytes);
}

static inline void encodeNumber(uint64_t number, Data& data) {
    if (number < 0x80) {
        // positive fixint
        data.push_back(static_cast<uint8_t>(number));
    } else if (number < 0x100) {
        // uint 8
        data.push_back(static_cast<uint8_t>(0xcc));
        data.push_back(static_cast<byte>(number));
    } else if (number < 0x10000) {
        // uint 16
        data.push_back(static_cast<uint8_t>(0xcd));
        encode16BE(static_cast<uint16_t>(number), data);
    } else if (number < 0x100000000) {
        // uint 32
        data.push_back(static_cast<uint8_t>(0xce));
        encode32BE(static_cast<uint32_t>(number), data);
    } else {
        // uint 64
        data.push_back(static_cast<uint8_t>(0xcf));
        encode64BE(number, data);
    }
}

static inline void encodeBytes(const Data& bytes, Data& data) {
    auto size = bytes.size();
    if (size < 0x100) {
        // bin 8
        data.push_back(static_cast<uint8_t>(0xc4));
        data.push_back(static_cast<uint8_t>(size));
    } else if (size < 0x10000) {
        // bin 16
        data.push_back(static_cast<uint8_t>(0xc5));
        encode16BE(static_cast<uint16_t>(size), data);
    } else if (size < 0x100000000) {
        // bin 32
        data.push_back(static_cast<uint8_t>(0xc6));
        encode32BE(static_cast<uint32_t>(size), data);
    } else {
        // too long binary
        return;
    }
    append(data, bytes);
}

} // namespace TW::Algorand
