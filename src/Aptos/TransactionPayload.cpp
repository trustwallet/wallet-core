// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Aptos/TransactionPayload.h>
#include <utility>

namespace TW::Aptos {

EntryFunction::EntryFunction(ModuleId module, Identifier function, std::vector<TypeTag> tyArgs, std::vector<Data> args) noexcept
    : mModule(std::move(module)), mFunction(std::move(function)), mTyArgs(std::move(tyArgs)), mArgs(std::move(args)) {
}

} // namespace TW::Aptos
