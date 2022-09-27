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
    serializer << static_cast<std::byte>(gResourceTag);
    return serializer.bytes;
}

StructTag::StructTag(Address accountAddress, Identifier module, Identifier name, std::vector<TypeTag> typeParams) noexcept
    : mAccountAddress(accountAddress), mModule(std::move(module)), mName(std::move(name)), mTypeParams(std::move(typeParams)) {
}

} // namespace TW::Aptos
