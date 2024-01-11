// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWHDVersion.h>

#include <emscripten/bind.h>

using namespace emscripten;

namespace TW::Wasm {

    class HDVersionExt {
    public:
        static auto isPublic(TWHDVersion version);
        static auto isPrivate(TWHDVersion version);
    };
}
