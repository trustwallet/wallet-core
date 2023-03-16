// Copyright © 2017-2023 Trust Wallet.
// Created by Clément Doumergue

// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <algorithm>
#include <concepts>
#include <iterator>
#include <optional>
#include <string_view>
#include <tuple>
#include <variant>

#include "Data.h"
#include "concepts/tw_concepts.h"

namespace TW::BCS {

/// Implementation of BCS encoding (as specified by the Diem project, see github.com/diem/bcs#detailed-specifications)

struct Serializer {
    Data bytes;

    void add_byte(std::byte b) noexcept {
        bytes.emplace_back(static_cast<Data::value_type>(b));
    }

    template <typename Iterator>
    void add_bytes(Iterator first, Iterator last) noexcept {
        std::transform(first, last, std::back_inserter(bytes), [](auto&& c) {
            return static_cast<Data::value_type>(c);
        });
    }

    void clear() noexcept {
        bytes.clear();
    }
};

struct uleb128 {
    uint32_t value;
};

namespace details {

template <typename T>
concept aggregate_struct = std::is_class_v<std::remove_reference_t<T>> && std::is_aggregate_v<std::remove_reference_t<T>>;

template <typename T>
concept map_container = requires(T t) {
                            typename T::key_type;
                            typename T::mapped_type;
                            { std::declval<const T>().size() } -> std::same_as<typename T::size_type>;
                        };

template <typename T>
    requires integral<T> ||
             floating_point<T> ||
             std::same_as<T, std::string_view> ||
             std::same_as<T, std::string> ||
             std::same_as<T, uleb128> ||
             aggregate_struct<T> ||
             map_container<T>
struct is_serializable {
    static constexpr auto value = true;
};

template <typename T>
struct is_serializable<std::optional<T>> {
    static constexpr auto value = is_serializable<T>::value;
};

template <typename... Ts>
struct is_serializable<std::tuple<Ts...>> {
    static constexpr auto value = (is_serializable<Ts>::value && ...);
};

template <typename T, typename U>
struct is_serializable<std::pair<T, U>> {
    static constexpr auto value = is_serializable<T>::value && is_serializable<U>::value;
};

template <typename T, std::size_t N>
struct is_serializable<std::array<T, N>> {
    static constexpr auto value = is_serializable<T>::value;
};

template <typename... Ts>
struct is_serializable<std::variant<Ts...>> {
    static constexpr auto value = (is_serializable<Ts>::value && ...);
};

template <integral T, std::size_t... Is>
Serializer& serialize_integral_impl(Serializer& stream, T t, std::index_sequence<Is...>) noexcept {
    const char* bytes = reinterpret_cast<const char*>(&t);
    // Add each byte in little-endian order
    return (stream << ... << static_cast<std::byte>(bytes[Is]));
}

template <typename T, std::size_t... Is>
Serializer& serialize_tuple_impl(Serializer& stream, const T& t, std::index_sequence<Is...>) noexcept {
    return (stream << ... << std::get<Is>(t));
}

template <typename T>
struct dependent_false {
    static constexpr auto value = false;
};

template <aggregate_struct T>
constexpr auto to_tuple(T&& t) {
    if constexpr (std::is_empty_v<T>) {
        return std::make_tuple();
    } else if constexpr (requires { [&t] { auto&& [a0] = t; }; }) {
        auto&& [a0] = std::forward<T>(t);
        return std::make_tuple(a0);
    } else if constexpr (requires { [&t] { auto&& [a0, a1] = t; }; }) {
        auto&& [a0, a1] = std::forward<T>(t);
        return std::make_tuple(a0, a1);
    } else if constexpr (requires { [&t] { auto&& [a0, a1, a2] = t; }; }) {
        auto&& [a0, a1, a2] = std::forward<T>(t);
        return std::make_tuple(a0, a1, a2);
    } else if constexpr (requires { [&t] { auto&& [a0, a1, a2, a3] = t; }; }) {
        auto&& [a0, a1, a2, a3] = std::forward<T>(t);
        return std::make_tuple(a0, a1, a2, a3);
    } else if constexpr (requires { [&t] { auto&& [a0, a1, a2, a3, a4] = t; }; }) {
        auto&& [a0, a1, a2, a3, a4] = std::forward<T>(t);
        return std::make_tuple(a0, a1, a2, a3, a4);
    } else if constexpr (requires { [&t] { auto&& [a0, a1, a2, a3, a4, a5] = t; }; }) {
        auto&& [a0, a1, a2, a3, a4, a5] = std::forward<T>(t);
        return std::make_tuple(a0, a1, a2, a3, a4, a5);
    } else {
        static_assert(dependent_false<T>::value, "the structure has more than 6 members");
    }
}

template <aggregate_struct T>
Serializer& serialize_struct_impl(Serializer& stream, const T& t) noexcept {
    return stream << to_tuple(t);
}
} // namespace details

template <typename T>
concept PrimitiveSerializable = details::is_serializable<T>::value;

template <typename T>
concept CustomSerializable = requires(T t) {
                                 { std::declval<Serializer&>() << t } -> std::same_as<Serializer&>;
                             };

template <typename T>
concept Serializable = PrimitiveSerializable<T> || CustomSerializable<T>;

Serializer& operator<<(Serializer& stream, std::byte b) noexcept;

template <integral T>
Serializer& operator<<(Serializer& stream, T t) noexcept {
    return details::serialize_integral_impl(stream, t, std::make_index_sequence<sizeof(T)>{});
}

Serializer& operator<<(Serializer& stream, uleb128 t) noexcept;

Serializer& operator<<(Serializer& stream, std::string_view sv) noexcept;

template <typename T>
Serializer& operator<<(Serializer& stream, const std::optional<T> o) noexcept {
    if (o.has_value()) {
        stream << true;
        stream << o.value();
    } else {
        stream << false;
    }
    return stream;
}

Serializer& operator<<(Serializer& stream, std::nullopt_t) noexcept;

template <typename... Ts>
Serializer& operator<<(Serializer& stream, const std::tuple<Ts...>& t) noexcept {
    return details::serialize_tuple_impl(stream, t, std::make_index_sequence<sizeof...(Ts)>{});
}

template <typename T, typename U>
Serializer& operator<<(Serializer& stream, const std::pair<T, U>& t) noexcept {
    return details::serialize_tuple_impl(stream, t, std::make_index_sequence<2>{});
}

template <details::aggregate_struct T>
Serializer& operator<<(Serializer& stream, const T& t) noexcept {
    return details::serialize_struct_impl(stream, t);
}

template <typename T>
Serializer& operator<<(Serializer& stream, const std::vector<T>& t) noexcept {
    stream << uleb128{static_cast<uint32_t>(t.size())};
    for (auto&& cur: t) {
        stream << cur;
    }
    return stream;
}

template <typename... Ts>
Serializer& operator<<(Serializer& stream, const std::variant<Ts...>& t) noexcept {
    stream << uleb128{static_cast<uint32_t>(t.index())};
    std::visit([&stream](auto&& value) { stream << value; }, t);
    return stream;
}

template <details::map_container T>
Serializer& operator<<(Serializer& stream, const T& t) noexcept {
    stream << uleb128{static_cast<uint32_t>(t.size())};
    for (auto&& [k, v] : t) {
        stream << std::make_tuple(k, v);
    }
    return stream;
}

} // namespace TW::BCS
