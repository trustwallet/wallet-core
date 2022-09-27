// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <Aptos/MoveTypes.h>
#include <HexCoding.h>

namespace TW::Aptos {

/// Call a Move entry function.
class EntryFunction {
public:
    explicit EntryFunction(ModuleId module, Identifier function, std::vector<TypeTag> tyArgs, std::vector<Data> args) noexcept;
    [[nodiscard]] const ModuleId& module() const noexcept { return mModule; }
    [[nodiscard]] const Identifier& function() const noexcept { return mFunction; }
    [[nodiscard]] const std::vector<TypeTag>& tyArgs() const noexcept { return mTyArgs; }
    [[nodiscard]] const std::vector<Data>& args() const noexcept { return mArgs; }

private:
    ModuleId mModule;
    Identifier mFunction;
    std::vector<TypeTag> mTyArgs;
    std::vector<Data> mArgs;
};

static BCS::Serializer& operator<<(BCS::Serializer& stream, const EntryFunction& entryFunction) noexcept {
    auto serializedModule = entryFunction.module().serialize();
    stream << entryFunction.module();
    stream << entryFunction.function();
    stream << entryFunction.tyArgs();
    stream << entryFunction.args();
    return stream;
}

class Script {

};

[[maybe_unused]] static BCS::Serializer& operator<<(BCS::Serializer& stream, [[maybe_unused]] const Script& script) noexcept {
    return stream;
}

class ModuleBundle {

};

[[maybe_unused]] static BCS::Serializer& operator<<(BCS::Serializer& stream, [[maybe_unused]] const ModuleBundle& moduleBundle) noexcept {
    return stream;
}

using TransactionPayload = std::variant<Script, ModuleBundle, EntryFunction>;

} // namespace TW::Aptos
