// Copyright © 2017-2023 Trust Wallet.
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
    [[nodiscard]] Data accessVector() const noexcept;
    [[nodiscard]] std::string string() const noexcept;
    [[nodiscard]] std::string shortString() const noexcept;

private:
    Address mAccountAddress;
    Identifier mName;
};

inline ModuleId gAptosAccountModule{Address::one(), "aptos_account"};
inline ModuleId gAptosCoinModule{Address::one(), "coin"};
inline ModuleId gAptosManagedCoinsModule{Address::one(), "managed_coin"};
inline ModuleId gAptosTokenTransfersModule{Address::three(), "token_transfers"};

BCS::Serializer& operator<<(BCS::Serializer& stream, const ModuleId& module) noexcept;

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

std::string TypeTagToString(const TypeTag& typeTag) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, const StructTag& st) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, Bool) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, U8) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, U64) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, U128) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, TAddress) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, TSigner) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, const Vector& t) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, const TStructTag& t) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, const TypeTag& t) noexcept;
static const TypeTag gTransferTag = {TypeTag::TypeTagVariant(TStructTag{.st = StructTag(Address::one(), "aptos_coin", "AptosCoin", {})})};
static const TypeTag gOfferNftTag = {TypeTag::TypeTagVariant(TStructTag{.st = StructTag(Address::three(), "token_transfers", "offer_script", {})})};

} // namespace TW::Aptos
