// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Aptos/MoveTypes.h>
#include <sstream>

namespace TW::Aptos {

Aptos::ModuleId::ModuleId(Address accountAddress, Identifier name) noexcept
    : mAccountAddress(accountAddress), mName(std::move(name)) {
}

Data ModuleId::accessVector() const noexcept {
    BCS::Serializer serializer;
    serializer << static_cast<std::byte>(gCodeTag) << mAccountAddress << mName;
    return serializer.bytes;
}

std::string ModuleId::string() const noexcept {
    std::stringstream ss;
    ss << mAccountAddress.string() << "::" << mName;
    return ss.str();
}

std::string ModuleId::shortString() const noexcept {
    std::stringstream ss;
    ss << "0x" << mAccountAddress.shortString() << "::" << mName;
    return ss.str();
}

Data StructTag::serialize(bool withResourceTag) const noexcept {
    BCS::Serializer serializer;
    if (withResourceTag)
    {
        serializer << gResourceTag;
    }
    serializer << mAccountAddress << mModule << mName << mTypeParams;
    return serializer.bytes;
}

StructTag::StructTag(Address accountAddress, Identifier module, Identifier name, std::vector<TypeTag> typeParams) noexcept
    : mAccountAddress(accountAddress), mModule(std::move(module)), mName(std::move(name)), mTypeParams(std::move(typeParams)) {
}
std::string StructTag::string() const noexcept {
    std::stringstream ss;
    ss << "0x" << mAccountAddress.shortString() << "::" << mModule << "::" << mName;
    if (!mTypeParams.empty()) {
        ss << "<";
        ss << TypeTagToString(*mTypeParams.begin());
        std::for_each(begin(mTypeParams) + 1, end(mTypeParams), [&ss](auto&& cur) {
            ss << ", " << TypeTagToString(cur);
        });
        ss << ">";
    }
    return ss.str();
}

BCS::Serializer& operator<<(BCS::Serializer& stream, Bool) noexcept {
    stream << Bool::value;
    return stream;
}
BCS::Serializer& operator<<(BCS::Serializer& stream, U8) noexcept {
    stream << U8::value;
    return stream;
}
BCS::Serializer& operator<<(BCS::Serializer& stream, U64) noexcept {
    stream << U64::value;
    return stream;
}
BCS::Serializer& operator<<(BCS::Serializer& stream, U128) noexcept {
    stream << U128::value;
    return stream;
}
BCS::Serializer& operator<<(BCS::Serializer& stream, TAddress) noexcept {
    stream << TAddress::value;
    return stream;
}
BCS::Serializer& operator<<(BCS::Serializer& stream, TSigner) noexcept {
    stream << TSigner::value;
    return stream;
}

BCS::Serializer& operator<<(BCS::Serializer& stream, const StructTag& st) noexcept {
    auto res = st.serialize();
    stream.add_bytes(begin(res), end(res));
    return stream;
}

BCS::Serializer& operator<<(BCS::Serializer& stream, const TStructTag& st) noexcept {
    stream << TStructTag::value;
    auto res = st.st.serialize(false);
    stream.add_bytes(begin(res), end(res));
    return stream;
}

BCS::Serializer& operator<<(BCS::Serializer& stream, const Vector& t) noexcept {
    stream << Vector::value;
    for (auto&& cur: t.tags) {
        stream << cur;
    }
    return stream;
}

BCS::Serializer& operator<<(BCS::Serializer& stream, const TypeTag& t) noexcept {
    std::visit([&stream](auto&& arg) { stream << arg; }, t.tags);
    return stream;
}

BCS::Serializer& operator<<(BCS::Serializer& stream, const ModuleId& module) noexcept {
    stream << module.address() << module.name();
    return stream;
}
std::string TypeTagToString(const TypeTag& typeTag) noexcept {
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

} // namespace TW::Aptos
