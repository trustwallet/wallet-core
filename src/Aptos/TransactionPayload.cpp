// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Aptos/TransactionPayload.h>
#include <utility>

namespace TW::Aptos {

EntryFunction::EntryFunction(ModuleId module, Identifier function, std::vector<TypeTag> tyArgs, std::vector<Data> args, nlohmann::json jsonArgs) noexcept
    : mModule(std::move(module)), mFunction(std::move(function)), mTyArgs(std::move(tyArgs)), mArgs(std::move(args)), mJsonArgs(std::move(jsonArgs)) {
}

BCS::Serializer& operator<<(BCS::Serializer& stream, const EntryFunction& entryFunction) noexcept {
    stream << entryFunction.module() << entryFunction.function() << entryFunction.tyArgs() << entryFunction.args();
    return stream;
}

nlohmann::json payloadToJson(const TransactionPayload& payload) {
    auto visit_functor = [](const TransactionPayload& value) -> nlohmann::json {
        if (auto* entryFunction = std::get_if<EntryFunction>(&value); entryFunction) {
            return entryFunction->json();
        } else {
            return {};
        }
    };

    return std::visit(visit_functor, payload);
}

BCS::Serializer& operator<<(BCS::Serializer& stream, [[maybe_unused]] const Script& script) noexcept {
    return stream;
}

BCS::Serializer& operator<<(BCS::Serializer& stream, [[maybe_unused]] const ModuleBundle& moduleBundle) noexcept {
    return stream;
}

nlohmann::json EntryFunction::json() const noexcept {
    nlohmann::json tyArgsJson = nlohmann::json::array();
    for (auto&& cur : mTyArgs) {
        tyArgsJson.emplace_back(TypeTagToString(cur));
    }
    // clang-format off
    nlohmann::json out = {
        {"type", "entry_function_payload"},
        {"function", mModule.shortString() + "::" + mFunction},
        {"type_arguments", tyArgsJson},
        {"arguments", mJsonArgs}
    };
    // clang-format on
    return out;
}

} // namespace TW::Aptos
