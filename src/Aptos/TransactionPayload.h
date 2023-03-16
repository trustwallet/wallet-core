// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <Aptos/MoveTypes.h>
#include <HexCoding.h>
#include <nlohmann/json.hpp>

namespace TW::Aptos {

/// Call a Move entry function.
class EntryFunction {
public:
    explicit EntryFunction(ModuleId module, Identifier function, std::vector<TypeTag> tyArgs, std::vector<Data> args, nlohmann::json jsonArgs = {}) noexcept;
    [[nodiscard]] const ModuleId& module() const noexcept { return mModule; }
    [[nodiscard]] const Identifier& function() const noexcept { return mFunction; }
    [[nodiscard]] const std::vector<TypeTag>& tyArgs() const noexcept { return mTyArgs; }
    [[nodiscard]] const std::vector<Data>& args() const noexcept { return mArgs; }
    [[nodiscard]] nlohmann::json json() const noexcept;
    static EntryFunction from_json(const nlohmann::json& json) noexcept;

private:
    ModuleId mModule;
    Identifier mFunction;
    std::vector<TypeTag> mTyArgs;
    std::vector<Data> mArgs;
    nlohmann::json mJsonArgs;
};


class Script {
};

class ModuleBundle {
};

BCS::Serializer& operator<<(BCS::Serializer& stream, const EntryFunction& entryFunction) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, const Script& script) noexcept;
BCS::Serializer& operator<<(BCS::Serializer& stream, const ModuleBundle& moduleBundle) noexcept;
using TransactionPayload = std::variant<Script, ModuleBundle, EntryFunction>;
nlohmann::json payloadToJson(const TransactionPayload& payload);

} // namespace TW::Aptos
