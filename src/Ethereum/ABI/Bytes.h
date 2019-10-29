// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../../Data.h"
#include "../../uint256.h"
#include "Numbers.h"

namespace TW::Ethereum {

// dynamic bytes

inline bool is_dynamic(Data) {
    return true;
}

inline std::size_t size(const Data& bytes) {
    return 32 + ((bytes.size() + 31) / 32) * 32;
}

void encode(const Data& bytes, Data& data);

inline std::string type_string(const Data& data) {
    return "bytes";
}

// static bytes

template <std::size_t N>
inline bool is_dynamic(std::array<byte, N>) {
    return false;
}

template <std::size_t N>
inline std::size_t size(const std::array<byte, N>& bytes) {
    return ((bytes.size() + 31) / 32) * 32;
}

template <std::size_t N>
inline void encode(const std::array<byte, N>& bytes, Data& data) {
    const auto count = std::min(std::size_t(32), bytes.size());
    const auto padding = ((count + 31) / 32) * 32 - count;
    data.insert(data.end(), bytes.begin(), bytes.begin() + count);
    append(data, Data(padding));
}

template <std::size_t N>
inline std::string type_string(const std::array<byte, N>& bytes) {
    return "bytes" + std::to_string(N);
}

// string

inline bool is_dynamic(std::string) {
    return true;
}

inline std::size_t size(const std::string& string) {
    return 32 + ((string.size() + 31) / 32) * 32;
}

inline void encode(const std::string& string, Data& data) {
    auto bytes = Data(string.begin(), string.end());
    encode(bytes, data);
}

inline std::string type_string(const std::string& data) {
    return "string";
}

} // namespace TW::Ethereum
