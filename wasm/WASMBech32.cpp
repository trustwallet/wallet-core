// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Bech32.h"
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;

EMSCRIPTEN_BINDINGS(wallet_Bech32_module) {
    function("bech32encode", &Bech32::encode);

    enum_<Bech32::ChecksumVariant>("ChecksumVariant")
        .value("None", Bech32::ChecksumVariant::None)
        .value("Bech32", Bech32::ChecksumVariant::Bech32)
        .value("Bech32M", Bech32::ChecksumVariant::Bech32M);
}
