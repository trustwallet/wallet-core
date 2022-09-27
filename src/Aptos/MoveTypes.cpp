// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Aptos/MoveTypes.h>

namespace TW::Aptos {

Aptos::ModuleId::ModuleId(Address accountAddress, Identifier name) noexcept
    : mAccountAddress(accountAddress), mName(std::move(name)) {
}

Data ModuleId::serialize() const noexcept {
    BCS::Serializer serializer;
    serializer.add_byte(static_cast<std::byte>(gCodeTag));
    serializer << mAccountAddress;
    serializer << mName;
    return serializer.bytes;
}

} // namespace TW::Aptos
