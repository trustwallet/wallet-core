// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../../Data.h"
#include "../../uint256.h"

namespace TW::Ethereum {

static constexpr std::size_t encodedIntSize = 32;

// uint256_t

inline bool is_dynamic(uint256_t) {
    return false;
}

inline std::size_t size(uint256_t) {
    return encodedIntSize;
}

void encode(uint256_t value, Data& data);

bool decode(const Data& encoded, uint256_t& decoded, size_t& offset_inout);

inline std::string type_string(uint256_t value) {
    return "uint256";
}

// int256_t

inline bool is_dynamic(int256_t) {
    return false;
}

inline std::size_t size(int256_t) {
    return encodedIntSize;
}

inline void encode(int256_t value, Data& data) {
    encode(static_cast<uint256_t>(value), data);
}

template<typename T>
inline bool decodeNumber(const Data& encoded, T& decoded, size_t& offset_inout) {
    uint256_t decodedUInt256;
    bool res = decode(encoded, decodedUInt256, offset_inout);
    decoded = static_cast<T>(decodedUInt256);
    return res;
}

inline bool decode(const Data& encoded, int256_t& decoded, size_t& offset_inout) {
    return decodeNumber<int256_t>(encoded, decoded, offset_inout);
}

inline std::string type_string(int256_t value) {
    return "int256";
}

// bool

inline bool is_dynamic(bool) {
    return false;
}

inline std::size_t size(bool) {
    return encodedIntSize;
}

void encode(bool v, Data& data);

inline bool decode(const Data& encoded, bool& decoded, size_t& offset_inout) {
    return decodeNumber<bool>(encoded, decoded, offset_inout);
}

inline std::string type_string(bool value) {
    return "bool";
}

// int32

inline bool is_dynamic(int32_t) {
    return false;
}

inline std::size_t size(int32_t) {
    return encodedIntSize;
}

inline void encode(int32_t v, Data& data) {
    encode(static_cast<uint256_t>(v), data);
}

inline bool decode(const Data& encoded, int32_t& decoded, size_t& offset_inout) {
    return decodeNumber<int32_t>(encoded, decoded, offset_inout);
}

inline std::string type_string(int32_t value) {
    return "int32";
}

// uint32

inline bool is_dynamic(uint32_t) {
    return false;
}

inline std::size_t size(uint32_t) {
    return encodedIntSize;
}

inline void encode(uint32_t v, Data& data) {
    encode(static_cast<uint256_t>(v), data);
}

inline bool decode(const Data& encoded, uint32_t& decoded, size_t& offset_inout) {
    return decodeNumber<uint32_t>(encoded, decoded, offset_inout);
}

inline std::string type_string(uint32_t value) {
    return "uint32";
}

// int64

inline bool is_dynamic(int64_t) {
    return false;
}

inline std::size_t size(int64_t) {
    return encodedIntSize;
}

inline void encode(int64_t v, Data& data) {
    encode(static_cast<uint256_t>(v), data);
}

inline bool decode(const Data& encoded, int64_t& decoded, size_t& offset_inout) {
    return decodeNumber<int64_t>(encoded, decoded, offset_inout);
}

inline std::string type_string(int64_t value) {
    return "int64";
}

// uint64

inline bool is_dynamic(uint64_t) {
    return false;
}

inline std::size_t size(uint64_t) {
    return encodedIntSize;
}

inline void encode(uint64_t v, Data& data) {
    encode(static_cast<uint256_t>(v), data);
}

inline bool decode(const Data& encoded, uint64_t& decoded, size_t& offset_inout) {
    return decodeNumber<uint64_t>(encoded, decoded, offset_inout);
}

inline std::string type_string(uint64_t value) {
    return "uint64";
}

bool decode(const Data& encoded, uint8_t& decoded, size_t& offset_inout);

} // namespace TW::Ethereum
