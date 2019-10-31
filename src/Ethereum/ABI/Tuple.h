// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../../Data.h"
#include "../../uint256.h"
#include "Numbers.h"

#include <string>
#include <tuple>
#include <type_traits>

namespace TW::Ethereum {

template <typename T, typename... Ts>
inline auto head(std::tuple<T, Ts...> t) {
    return std::get<0>(t);
}

template <std::size_t... Ns, typename... Ts>
inline auto tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t) {
    return std::make_tuple(std::get<Ns + 1u>(t)...);
}

template <typename... Ts>
inline auto tail(std::tuple<Ts...> t) {
    return tail_impl(std::make_index_sequence<sizeof...(Ts) - 1u>(), t);
}

inline bool is_dynamic(const std::tuple<>& tuple) {
    return false;
}

template <typename First, typename... T>
inline bool is_dynamic(const std::tuple<First, T...>& tuple) {
    return is_dynamic(head(tuple)) || is_dynamic(tail(tuple));
}

inline std::size_t head_size(const std::tuple<>& tuple) {
    return 0;
}

template <typename First, typename... T>
inline std::size_t head_size(const std::tuple<First, T...>& tuple) {
    const auto s = is_dynamic(head(tuple)) ? 32 : size(head(tuple));
    return s + head_size(tail(tuple));
}

inline std::size_t size(const std::tuple<>& tuple) {
    return 0;
}

template <typename First, typename... T>
inline std::size_t size(const std::tuple<First, T...>& tuple) {
    return size(head(tuple)) + size(tail(tuple));
}

inline void encode_small(const std::tuple<>& tuple, std::size_t headSize, std::size_t& offset,
                  Data& data) {}

template <typename First, typename... T>
inline void encode_small(const std::tuple<First, T...>& tuple, std::size_t headSize, std::size_t& offset,
                  Data& data) {
    auto value = head(tuple);
    if (is_dynamic(value)) {
        encode(uint256_t(headSize + offset), data);
        offset += size(value);
    } else {
        encode(value, data);
    }

    encode_small(tail(tuple), headSize, offset, data);
}

inline void encode_dynamic(const std::tuple<>& tuple, Data& data) {}

template <typename First, typename... T>
inline void encode_dynamic(const std::tuple<First, T...>& tuple, Data& data) {
    auto value = head(tuple);
    if (is_dynamic(value)) {
        encode(value, data);
    }

    encode_dynamic(tail(tuple), data);
}

template <typename... T>
inline void encode(const std::tuple<T...>& tuple, Data& data) {
    std::size_t headSize = head_size(tuple);

    std::size_t dynamicOffset = 0;
    encode_small(tuple, headSize, dynamicOffset, data);
    encode_dynamic(tuple, data);
}

inline std::string type_string(const std::tuple<>& tuple) {
    return "";
}

template <typename... T>
inline std::string type_string(const std::tuple<T...>& tuple) {
    std::string string = type_string(head(tuple));
    if (std::tuple_size<std::tuple<T...>>::value > 1) {
        string += "," + type_string(tail(tuple));
    }
    return string;
}

inline bool decodeTupleSmall(const Data& encoded, std::tuple<>& tuple_inout, size_t& offset_inout) { return true; }

/// Process small values and extract indices for larger values
template <typename First, typename... T>
inline bool decodeTupleSmall(const Data& encoded, std::tuple<First, T...>& tuple_inout, size_t& offset_inout) {
    auto headTuple = head(tuple_inout);
    if (is_dynamic(headTuple)) {
        uint256_t index;
        if (!decode(encoded, index, offset_inout)) { return false; }
        // index is read but not used
    } else {
        if (!decode(encoded, headTuple, offset_inout)) { return false; }
    }
    // process recursively, must recreate L-value
    std::tuple tailTuple = tail(tuple_inout);
    bool res = decodeTupleSmall(encoded, tailTuple, offset_inout);
    tuple_inout = std::tuple_cat(std::make_tuple(headTuple), tailTuple);
    return res;
}

inline bool decodeTupleLarge(const Data& encoded, std::tuple<>& tuple_inout, size_t& offset_inout) { return true; }

/// Process large values
template <typename First, typename... T>
inline bool decodeTupleLarge(const Data& encoded, std::tuple<First, T...>& tuple_inout, size_t& offset_inout) {
    auto headTuple = head(tuple_inout);
    if (is_dynamic(headTuple)) {
        if (!decode(encoded, headTuple, offset_inout)) { return false; }
    }
    // process recursively, must recreate L-value
    std::tuple tailTuple = tail(tuple_inout);
    bool res = decodeTupleLarge(encoded, tailTuple, offset_inout);
    tuple_inout = std::tuple_cat(std::make_tuple(headTuple), tailTuple);
    return res;
}

/// Decode a tuple.  The tuple has to be provided, the types must be known.  The provided tuple is filled with values.
template <typename... T>
inline bool decode(const Data& encoded, std::tuple<T...>& tuple_inout, size_t& offset_inout) {
    // pass 1: process small values
    if (!decodeTupleSmall(encoded, tuple_inout, offset_inout)) { return false; }
    // pass 2: process large values
    return decodeTupleLarge(encoded, tuple_inout, offset_inout);
}

} // namespace TW::Ethereum
