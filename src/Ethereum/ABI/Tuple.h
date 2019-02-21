// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../../Data.h"

#include <tuple>
#include <type_traits>

namespace TW {
namespace Ethereum {

template <typename T , typename... Ts>
auto head(std::tuple<T, Ts...> t) {
    return std::get<0>(t);
}

template <std::size_t... Ns , typename... Ts>
auto tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t) {
    return std::make_tuple(std::get<Ns + 1u>(t)...);
}

template <typename... Ts>
auto tail(std::tuple<Ts...> t) {
    return tail_impl(std::make_index_sequence<sizeof...(Ts) - 1u>() , t);
}

bool is_dynamic(const std::tuple<>& tuple) {
    return false;
}

template <typename First, typename... T>
bool is_dynamic(const std::tuple<First, T...>& tuple) {
    return is_dynamic(head(tuple)) || is_dynamic(tail(tuple));
}

std::size_t head_size(const std::tuple<>& tuple) {
    return 0;
}

template <typename First, typename... T>
std::size_t head_size(const std::tuple<First, T...>& tuple) {
    const auto s = is_dynamic(head(tuple)) ? 32 : size(head(tuple));
    return s + head_size(tail(tuple));
}

std::size_t size(const std::tuple<>& tuple) {
    return 0;
}

template <typename First, typename... T>
std::size_t size(const std::tuple<First, T...>& tuple) {
    return size(head(tuple)) + size(tail(tuple));
}

void encode_small(const std::tuple<>& tuple, std::size_t headSize, std::size_t& offset, Data& data) {}

template <typename First, typename... T>
void encode_small(const std::tuple<First, T...>& tuple, std::size_t headSize, std::size_t& offset, Data& data) {
    auto value = head(tuple);
    if (is_dynamic(value)) {
        encode(uint256_t(headSize + offset), data);
        offset += size(value);
    } else {
        encode(value, data);
    }

    encode_small(tail(tuple), headSize, offset, data);
}

void encode_dynamic(const std::tuple<>& tuple, Data& data) {}

template <typename First, typename... T>
void encode_dynamic(const std::tuple<First, T...>& tuple, Data& data) {
    auto value = head(tuple);
    if (is_dynamic(value)) {
        encode(value, data);
    }

    encode_dynamic(tail(tuple), data);
}

template <typename... T>
void encode(const std::tuple<T...>& tuple, Data& data) {
    std::size_t headSize = head_size(tuple);

    std::size_t dynamicOffset = 0;
    encode_small(tuple, headSize, dynamicOffset, data);
    encode_dynamic(tuple, data);
}

std::string type_string(const std::tuple<>& tuple) {
    return "";
}

template <typename... T>
std::string type_string(const std::tuple<T...>& tuple) {
    std::string string = type_string(head(tuple));
    if (std::tuple_size<std::tuple<T...>>::value > 1) {
        string += "," + type_string(tail(tuple));
    }
    return string;
}

}} // namespace
