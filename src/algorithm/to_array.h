// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <array>       //< std::array
#include <cstddef>     //< std::size_t
#include <type_traits> //< std::is_same

namespace TW {

template <typename T, typename Iter, std::size_t... Is>
constexpr auto to_array(Iter& iter, std::index_sequence<Is...>) -> std::array<T, sizeof...(Is)> {
    return {{((void)Is, T(*iter++))...}};
}

template <std::size_t N, typename U = void, typename Iter,
          typename V = typename std::iterator_traits<Iter>::value_type,
          typename T = std::conditional_t<std::is_same<U, void>{}, V, U>>
constexpr auto to_array(Iter iter) -> std::array<T, N> {
    return to_array<T>(iter, std::make_index_sequence<N>{});
}

} // namespace TW
