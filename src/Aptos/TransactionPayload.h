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
    stream.add_bytes(begin(serializedModule), end(serializedModule));
    stream << entryFunction.function() << entryFunction.tyArgs() << entryFunction.args();
    return stream;
}

using TransactionPayload = std::variant<EntryFunction>;

[[maybe_unused]] static BCS::Serializer& operator<<(BCS::Serializer& stream, const TransactionPayload& entryFunction) noexcept {
    auto visit_functor = [&stream](const TransactionPayload& value) {
        if (auto* entryFunction = std::get_if<EntryFunction>(&value); entryFunction != nullptr) {
            stream << *entryFunction;
        }
    };

    std::visit(visit_functor, entryFunction);
    return stream;
}

} // namespace TW::Aptos
