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
