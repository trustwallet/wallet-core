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
            .class_function("isPublic", &HDVersionExt::isPublic, allow_raw_pointers())
            .class_function("isPrivate", &HDVersionExt::isPrivate, allow_raw_pointers());
    };
}
