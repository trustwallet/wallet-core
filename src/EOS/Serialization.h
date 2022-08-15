#pragma once

#include <nlohmann/json.hpp>

#include "Data.h"
#include "../BinaryCoding.h"

#include <vector>
#include <set>
#include <string>

namespace TW::EOS {
inline void encodeVarInt64(uint64_t x, Data& os) {
    // 64-bit int would take at most 10 bytes as a varint
    static const int maxBytes = 10;
    uint8_t bytes[maxBytes];

    int lastNonZeroByte = 0;
    for (int i = 0; i < maxBytes; ++i) {
        bytes[i] = (x & 0x7F);
        if (bytes[i]) {
            lastNonZeroByte = i;
        }

        // set the first bit
        bytes[i] |= 0x80;
        x >>= 7;
    }

    // unset the first bit of the last byte
    bytes[lastNonZeroByte] &= 0x7F;

    os.insert(os.end(), bytes, bytes + lastNonZeroByte + 1);
}

inline void encodeVarInt32(uint32_t x, Data& os) {
    encodeVarInt64(static_cast<uint64_t>(x), os);
}

inline void encodeString(const std::string& s, Data& os) {
    size_t size = s.size();
    encodeVarInt64(size, os);
    os.insert(os.end(), s.data(), s.data() + size);
}

template<typename Collection>
inline void encodeCollection(const Collection& collection, Data& os) {
    encodeVarInt64(std::size(collection), os);
    for (const auto& item : collection) {
        item.serialize(os);
    }
}

template<typename Collection>
inline void encodePointerCollection(const Collection& collection, Data& os) {
    encodeVarInt64(std::size(collection), os);
    for (const auto& item : collection) {
        item->serialize(os);
    }
}

using json = nlohmann::json;

template<typename Collection>
inline json encodeCollection(const Collection& collection) {
    json array = json::array();
    for (const auto& item : collection) {
        array.push_back(item.serialize());
    }

    return array;
}

template<typename Collection>
inline json encodePointerCollection(const Collection& collection) {
    json array = json::array();
    for (const auto& item : collection) {
        array.push_back(item->serialize());
    }

    return array;
}
} // namespace