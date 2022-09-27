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
using Identifier = std::string;

class ModuleId {
public:
    ///< Constructor
    ModuleId(Address accountAddress, Identifier name) noexcept;

    ///< Getters
    [[nodiscard]] const std::string& name() const noexcept { return mName; }
    [[nodiscard]] const Address& address() const noexcept { return mAccountAddress; }
    [[nodiscard]] Data serialize() const noexcept;

private:
    Address mAccountAddress;
    Identifier mName;
};

} // namespace TW::Aptos
