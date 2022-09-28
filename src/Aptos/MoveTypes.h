// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Aptos/Address.h"
#include "BCS.h"
#include <sstream>

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

static BCS::Serializer& operator<<(BCS::Serializer& stream, const ModuleId& module) noexcept {
    stream << module.address() << module.name();
    return stream;
}

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
    [[nodiscard]] Data serialize(bool withResourceTag = true) const noexcept;
    [[nodiscard]] ModuleId moduleID() const noexcept { return {mAccountAddress, mName}; };
    [[nodiscard]] std::string string() const noexcept;

private:
    Address mAccountAddress;
    Identifier mModule;
    Identifier mName;
    std::vector<TypeTag> mTypeParams;
};

// C++ limitation, the first StructTag will serialize with ResourceTag, the inner one will use the value 7 instead. Tweaking by wrapping the struct
struct TStructTag {
    static constexpr std::uint8_t value = 7;
    StructTag st;
};

struct TypeTag {
    using TypeTagVariant = std::variant<Bool, U8, U64, U128, TAddress, TSigner, Vector, StructTag, TStructTag>;
    TypeTagVariant tags;
};

static std::string TypeTagToString(const TypeTag& typeTag) noexcept {
    auto visit_functor = [](const TypeTag::TypeTagVariant& value) -> std::string {
        if (std::holds_alternative<Bool>(value)) {
            return "bool";
        } else if (std::holds_alternative<U8>(value)) {
            return "u8";
        } else if (std::holds_alternative<U64>(value)) {
            return "u64";
        } else if (std::holds_alternative<U128>(value)) {
            return "u128";
        } else if (std::holds_alternative<TAddress>(value)) {
            return "address";
        } else if (std::holds_alternative<TSigner>(value)) {
            return "signer";
        } else if (auto* vectorData = std::get_if<Vector>(&value); vectorData != nullptr && !vectorData->tags.empty()) {
            std::stringstream ss;
            ss << "vector<" << TypeTagToString(*vectorData->tags.begin()) << ">";
            return ss.str();
        } else if (auto* structData = std::get_if<StructTag>(&value); structData) {
            return structData->string();
        } else if (auto* tStructData = std::get_if<TStructTag>(&value); tStructData) {
            return tStructData->st.string();
        } else {
            return "";
        }
    };

    return std::visit(visit_functor, typeTag.tags);
}

BCS::Serializer&
operator<<(BCS::Serializer& stream, const StructTag& st) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, Bool) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, U8) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, U64) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, U128) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, TAddress) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, TSigner) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, const Vector& t) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, const TStructTag& t) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, const TypeTag& t) noexcept;

static const TypeTag gTransferTag = {TypeTag::TypeTagVariant(TStructTag{.st = StructTag(gAddressOne, "aptos_coin", "AptosCoin", {})})};
} // namespace TW::Aptos
