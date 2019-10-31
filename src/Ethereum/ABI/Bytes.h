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

/// Compute size when padded to 32-byte boundary; round up to multiple of 32.
inline size_t paddedTo32(size_t len) { return ((len + 31) / 32) * 32; }

/// Compute padding needed to next 32-byte boundary.
inline size_t padNeeded32(size_t len) { return ((len + 31) / 32) * 32 - len; }

// dynamic bytes (Data)

inline bool is_dynamic(Data) {
    return true;
}

inline std::size_t size(const Data& bytes) {
    return 32 + paddedTo32(bytes.size());
}

void encode(const Data& bytes, Data& data);

bool decode(const Data& encoded, Data& decoded, size_t& offset_inout);

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
    return paddedTo32(bytes.size());
}

template <std::size_t N>
inline void encode(const std::array<byte, N>& bytes, Data& data) {
    const auto count = std::min(std::size_t(32), bytes.size());
    const auto padding = padNeeded32(count);
    data.insert(data.end(), bytes.begin(), bytes.begin() + count);
    append(data, Data(padding));
}

template <std::size_t N>
inline bool decode(const Data& encoded, std::array<byte, N>& decoded, size_t& offset_inout) {
    size_t origOffset = offset_inout;
    if (encoded.size() < offset_inout + N) {
        // not enough data
        return false;
    }
    std::copy(encoded.begin() + offset_inout, encoded.begin() + (offset_inout + N), decoded.begin());
    offset_inout += N;
    // padding
    offset_inout = origOffset + paddedTo32(offset_inout - origOffset);
    return true;
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
    return 32 + paddedTo32(string.size());
}

inline void encode(const std::string& string, Data& data) {
    auto bytes = Data(string.begin(), string.end());
    encode(bytes, data);
}

bool decode(const Data& encoded, std::string& decoded, size_t& offset_inout);

inline std::string type_string(const std::string& data) {
    return "string";
}

} // namespace TW::Ethereum
