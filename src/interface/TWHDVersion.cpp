// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHDVersion.h>

#pragma clang diagnostic push
#pragma clang diagnostic fatal "-Wswitch"

bool TWHDVersionIsPublic(enum TWHDVersion version) {
    switch (version) {
    case TWHDVersionXPUB:
    case TWHDVersionYPUB:
    case TWHDVersionZPUB:
    case TWHDVersionLTUB:
    case TWHDVersionMTUB:
    case TWHDVersionDPUB:
    case TWHDVersionDGUB:
        return true;

    case TWHDVersionNone:
    default:
        return false;
    }
}

bool TWHDVersionIsPrivate(enum TWHDVersion version) {
    switch (version) {
    case TWHDVersionXPRV:
    case TWHDVersionYPRV:
    case TWHDVersionZPRV:
    case TWHDVersionLTPV:
    case TWHDVersionMTPV:
    case TWHDVersionDPRV:
    case TWHDVersionDGPV:
        return true;

    case TWHDVersionNone:
    default:
        return false;
    }
}

#pragma clang diagnostic pop
