// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
