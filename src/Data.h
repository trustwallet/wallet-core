// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <array>

namespace TW {

using byte = std::uint8_t;
using Data = std::vector<byte>;

typedef std::vector<std::pair<Data, Data>> HashPubkeyList;
typedef std::vector<std::pair<Data, Data>> SignaturePubkeyList;

inline void pad_left(Data& data, const uint32_t size) {
    data.insert(data.begin(), size - data.size(), 0);
}

template<typename It>
inline Data data(It&& begin, It&& end) {
    return Data(begin, end);
}

template<typename Collection>
inline Data data_from(const Collection& collection) {
    Data out;
    out.reserve(collection.size());
    for (auto&& cur : collection) {
        out.emplace_back(uint8_t(cur));
    }
    return out;
}

inline Data data(const std::string& data) {
    return Data(data.begin(), data.end());
}

inline Data data(const byte* data, size_t size) {
    return Data(data, data + size);
}

inline void append(Data& data, const Data& suffix) {
    data.insert(data.end(), suffix.begin(), suffix.end());
}

inline Data concat(const Data& data, const Data& suffix) {
    Data out = data;
    append(out, suffix);
    return out;
}

inline void append(Data& data, const byte suffix) {
    data.push_back(suffix);
}

/// Return a part (subdata) from the requested start position and size of the input data.
Data subData(const Data& data, size_t startIndex, size_t length);

/// Return the tail part (subdata) from the requested start position of the input data.
Data subData(const Data& data, size_t startIndex);

/// Determines if a byte array has a specific prefix.
template <typename T>
inline bool has_prefix(const Data& data, T& prefix) {
    return std::equal(prefix.begin(), prefix.end(), data.begin(), data.begin() + std::min(data.size(), prefix.size()));
}

// Custom hash function for `Data` type.
struct DataHash {
    std::size_t operator()(const Data& data) const {
        // Create a string_view from the vector's data.
        std::string_view ss(reinterpret_cast<const char*>(data.data()), data.size());
        // Use the hash function for std::string_view
        return std::hash<std::string_view>{}(ss);
    }
};

} // namespace TW
