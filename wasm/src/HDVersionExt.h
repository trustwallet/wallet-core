// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
