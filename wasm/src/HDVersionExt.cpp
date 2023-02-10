// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHDVersion.h>

#include <emscripten/bind.h>

using namespace emscripten;

#include "HDVersionExt.h"

namespace TW::Wasm {

    auto HDVersionExt::isPublic(TWHDVersion version) {
        return TWHDVersionIsPublic(version);
    }
    auto HDVersionExt::isPrivate(TWHDVersion version) {
        return TWHDVersionIsPrivate(version);
    }

    EMSCRIPTEN_BINDINGS(Wasm_HDVersionExt) {
        class_<HDVersionExt>("HDVersionExt")
            .class_function("isPublic", &HDVersionExt::isPublic)
            .class_function("isPrivate", &HDVersionExt::isPrivate);
    };
}
