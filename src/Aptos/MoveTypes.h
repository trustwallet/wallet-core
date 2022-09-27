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

struct Bool {};
struct U8 {};
struct U64 {};
struct U128 {};
struct TAddress {};
struct TSigner {};
struct TypeTag;

class StructTag {
public:
    explicit StructTag(Address accountAddress, Identifier module, Identifier name, std::vector<TypeTag> typeParams) noexcept;
    [[nodiscard]] Data serialize() const noexcept;
    [[nodiscard]] ModuleId moduleID() const noexcept { return ModuleId(mAccountAddress, mName); };

private:
    Address mAccountAddress;
    Identifier mModule;
    Identifier mName;
    std::vector<TypeTag> mTypeParams;
};

struct TypeTag {
    using TypeTagVariant = std::variant<Bool, U8, U64, U128, TAddress, TSigner, std::vector<TypeTag>, StructTag>;
    TypeTagVariant tags;
};

} // namespace TW::Aptos
