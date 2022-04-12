// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#pragma once

#include <TrustWalletCore/TWData.h>
#include "Data.h"

namespace TW::Wasm {

auto TWDataToStd(TWData *_Nonnull data) -> Data {
    auto* v = reinterpret_cast<const Data*>(data);
    Data result = Data(v->begin(), v->end());
    TWDataDelete(data);
    return result;
}

} // namespace TW::Wasm
