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

EMSCRIPTEN_BINDINGS(wallet_hdversion_module) {
    enum_<TWHDVersion>("TWHDVersion")
        .value("TWHDVersionNone", TWHDVersion::TWHDVersionNone)
        // Bitcoin
        .value("TWHDVersionXPUB", TWHDVersion::TWHDVersionXPUB)
        .value("TWHDVersionXPRV", TWHDVersion::TWHDVersionXPRV)
        .value("TWHDVersionYPUB", TWHDVersion::TWHDVersionYPUB)
        .value("TWHDVersionYPRV", TWHDVersion::TWHDVersionYPRV)
        .value("TWHDVersionZPUB", TWHDVersion::TWHDVersionZPUB)
        .value("TWHDVersionZPRV", TWHDVersion::TWHDVersionZPRV)
        // Litecoin
        .value("TWHDVersionLTUB", TWHDVersion::TWHDVersionLTUB)
        .value("TWHDVersionLTPV", TWHDVersion::TWHDVersionLTPV)
        .value("TWHDVersionMTUB", TWHDVersion::TWHDVersionMTUB)
        .value("TWHDVersionMTPV", TWHDVersion::TWHDVersionMTPV)
        // Decred
        .value("TWHDVersionDPUB", TWHDVersion::TWHDVersionDPUB)
        .value("TWHDVersionDPRV", TWHDVersion::TWHDVersionDPRV)
        // Dogecoin
        .value("TWHDVersionDGUB", TWHDVersion::TWHDVersionDGUB)
        .value("TWHDVersionDGPV", TWHDVersion::TWHDVersionDGPV);

    function("TWHDVersionIsPublic", &TWHDVersionIsPublic);
    function("TWHDVersionIsPrivate", &TWHDVersionIsPrivate);
}
