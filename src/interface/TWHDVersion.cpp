// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
