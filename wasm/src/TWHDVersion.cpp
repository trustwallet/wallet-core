// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include <TrustWalletCore/TWHDVersion.h>

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(WASM_TWHDVersion) {
    enum_<TWHDVersion>("HDVersion")
        .value("None", TWHDVersion::TWHDVersionNone)
        // Bitcoin
        .value("XPUB", TWHDVersion::TWHDVersionXPUB)
        .value("XPRV", TWHDVersion::TWHDVersionXPRV)
        .value("YPUB", TWHDVersion::TWHDVersionYPUB)
        .value("YPRV", TWHDVersion::TWHDVersionYPRV)
        .value("ZPUB", TWHDVersion::TWHDVersionZPUB)
        .value("ZPRV", TWHDVersion::TWHDVersionZPRV)
        // Litecoin
        .value("LTUB", TWHDVersion::TWHDVersionLTUB)
        .value("LTPV", TWHDVersion::TWHDVersionLTPV)
        .value("MTUB", TWHDVersion::TWHDVersionMTUB)
        .value("MTPV", TWHDVersion::TWHDVersionMTPV)
        // Decred
        .value("DPUB", TWHDVersion::TWHDVersionDPUB)
        .value("DPRV", TWHDVersion::TWHDVersionDPRV)
        // Dogecoin
        .value("DGUB", TWHDVersion::TWHDVersionDGUB)
        .value("DGPV", TWHDVersion::TWHDVersionDGPV);

    function("HDVersionIsPublic", &TWHDVersionIsPublic);
    function("HDVersionIsPrivate", &TWHDVersionIsPrivate);
}
