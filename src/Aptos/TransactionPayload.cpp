// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "rust/bindgen/WalletCoreRSBindgen.h"
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
        {"arguments", mJsonArgs.empty() ? nlohmann::json::array() : mJsonArgs}
    };
    // clang-format on
    return out;
}

EntryFunction EntryFunction::from_json(const nlohmann::json& payload) noexcept {
    auto splitFunctor = [](std::string s, std::string_view delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.size();
        std::string token;
        std::vector<std::string> output;

        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
            token = s.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            output.emplace_back(token);
        }

        output.emplace_back(s.substr(pos_start));
        return output;
    };
    auto functionSplitted = splitFunctor(payload.at("function").get<std::string>(), "::");
    auto moduleId = ModuleId(Address(functionSplitted[0]), functionSplitted[1]);
    std::vector<Data> args;
    for (auto&& cur : payload.at("arguments")) {
        auto curStr = cur.get<std::string>();
        auto res = Rust::parse_function_argument_to_bcs(curStr.c_str());
        if (res.code != Rust::OK_CODE) {
            // TODO consider exiting this function.
            args.emplace_back();
            continue;
        }
        args.emplace_back(parse_hex(res.result));
        Rust::free_string(res.result);
    }

    std::vector<TypeTag> tags;

    for (auto&& cur : payload.at("type_arguments")) {
        auto curStr = cur.get<std::string>();
        switch (Rust::parse_type_tag(curStr.c_str())) {
        case Rust::ETypeTag::Bool:
            break;
        case Rust::ETypeTag::U8:
            break;
        case Rust::ETypeTag::U64:
            break;
        case Rust::ETypeTag::U128:
            break;
        case Rust::ETypeTag::Address:
            break;
        case Rust::ETypeTag::Signer:
            break;
        case Rust::ETypeTag::Vector:
            break;
        case Rust::ETypeTag::Struct: {
            auto structSplitted = splitFunctor(curStr, "::");
            auto addr = Address(structSplitted[0]);
            TypeTag tag = {TypeTag::TypeTagVariant(TStructTag{.st = StructTag(addr, structSplitted[1], structSplitted[2], {})})};
            tags.emplace_back(tag);
            break;
        }
        case Rust::ETypeTag::Error:
            break;
        default:
            break;
        }
    }

    return EntryFunction(moduleId, functionSplitted[2], tags, {args}, payload.at("arguments"));
}

} // namespace TW::Aptos
