// Copyright © 2017-2022 Trust Wallet.
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

Data ModuleId::serialize() const noexcept {
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

Data StructTag::serialize() const noexcept {
    BCS::Serializer serializer;
    serializer << static_cast<std::byte>(gResourceTag) << mAccountAddress << mModule << mName << mTypeParams;
    /*for (auto&& cur: mTypeParams) {
        std::visit([&serializer](auto&& arg) { serializer << arg; }, cur.tags);
    }*/
    return serializer.bytes;
}

StructTag::StructTag(Address accountAddress, Identifier module, Identifier name, std::vector<TypeTag> typeParams) noexcept
    : mAccountAddress(accountAddress), mModule(std::move(module)), mName(std::move(name)), mTypeParams(std::move(typeParams)) {
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
BCS::Serializer& operator<<(BCS::Serializer& stream, StructTag st) noexcept {
    auto res = st.serialize();
    stream.add_bytes(begin(res), end(res));
    return stream;
}

BCS::Serializer& operator<<(BCS::Serializer& stream, const TypeTag& t) noexcept {
    std::visit([&stream](auto&& arg) { stream << arg; }, t.tags);
    return stream;
}
} // namespace TW::Aptos
