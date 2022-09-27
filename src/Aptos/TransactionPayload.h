// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <Aptos/MoveTypes.h>

namespace TW::Aptos {

/// Call a Move entry function.
class EntryFunction {
public:
    explicit EntryFunction(ModuleId module, Identifier function, std::vector<TypeTag> tyArgs, std::vector<Data> args) noexcept;
    [[nodiscard]] const ModuleId& module() const noexcept { return mModule;}
private:
    ModuleId mModule;
    Identifier mFunction;
    std::vector<TypeTag> mTyArgs;
    std::vector<Data> mArgs;
};

using TransactionPayload = std::variant<EntryFunction>;

} // namespace TW::Aptos
