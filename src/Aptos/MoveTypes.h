// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Aptos/Address.h"
#include "BCS.h"

namespace TW::Aptos {

constexpr std::uint8_t gCodeTag{0};
constexpr std::uint8_t gResourceTag{1};
using Identifier = std::string;

class ModuleId {
public:
    ///< Constructor
    ModuleId(Address accountAddress, Identifier name) noexcept;

    ///< Getters
    [[nodiscard]] const std::string& name() const noexcept { return mName; }
    [[nodiscard]] const Address& address() const noexcept { return mAccountAddress; }
    [[nodiscard]] Data serialize() const noexcept;
    [[nodiscard]] std::string string() const noexcept;
    [[nodiscard]] std::string shortString() const noexcept;

private:
    Address mAccountAddress;
    Identifier mName;
};

struct TypeTag;

struct Bool {
    static constexpr std::uint8_t value = 0;
};
struct U8 {
    static constexpr std::uint8_t value = 1;
};
struct U64 {
    static constexpr std::uint8_t value = 2;
};
struct U128 {
    static constexpr std::uint8_t value = 3;
};
struct TAddress {
    static constexpr std::uint8_t value = 4;
};
struct TSigner {
    static constexpr std::uint8_t value = 5;
};
struct Vector {
    static constexpr std::uint8_t value = 6;
    std::vector<TypeTag> tags;
};

class StructTag {
public:
    explicit StructTag(Address accountAddress, Identifier module, Identifier name, std::vector<TypeTag> typeParams) noexcept;
    [[nodiscard]] Data serialize() const noexcept;
    [[nodiscard]] ModuleId moduleID() const noexcept { return {mAccountAddress, mName}; };

private:
    Address mAccountAddress;
    Identifier mModule;
    Identifier mName;
    std::vector<TypeTag> mTypeParams;
};

struct TypeTag {
    using TypeTagVariant = std::variant<Bool, U8, U64, U128, TAddress, TSigner, Vector, StructTag>;
    TypeTagVariant tags;
};

BCS::Serializer& operator<<(BCS::Serializer& stream, StructTag st) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, Bool) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, U8) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, U64) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, U128) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, TAddress) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, TSigner) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, Vector t) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, const TypeTag& t) noexcept;

// TODO: find a way to not hit the aggregate one
/*template <typename T>
    requires std::same_as<T, Bool> ||
             std::same_as<T, U8> ||
             std::same_as<T, U64> ||
             std::same_as<T, U128> ||
             std::same_as<T, TAddress> ||
             std::same_as<T, TSigner>
struct is_type_tag {
    static constexpr auto value = true;
};

template <typename T>
concept IsTypeTag = is_type_tag<T>::value;

//template <IsTypeTag T>
*/
} // namespace TW::Aptos
