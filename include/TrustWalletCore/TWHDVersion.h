// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Registered HD version bytes
///
/// \see https://github.com/satoshilabs/slips/blob/master/slip-0132.md
TW_EXPORT_ENUM(uint32_t)
enum TWHDVersion {
    TWHDVersionNone = 0,

    // Bitcoin
    TWHDVersionXPUB = 0x0488b21e,
    TWHDVersionXPRV = 0x0488ade4,
    TWHDVersionYPUB = 0x049d7cb2,
    TWHDVersionYPRV = 0x049d7878,
    TWHDVersionZPUB = 0x04b24746,
    TWHDVersionZPRV = 0x04b2430c,
    TWHDVersionVPUB = 0x045f1cf6,
    TWHDVersionVPRV = 0x045f18bc,
    TWHDVersionTPUB = 0x043587cf,
    TWHDVersionTPRV = 0x04358394,

    // Litecoin
    TWHDVersionLTUB = 0x019da462,
    TWHDVersionLTPV = 0x019d9cfe,
    TWHDVersionMTUB = 0x01b26ef6,
    TWHDVersionMTPV = 0x01b26792,
    TWHDVersionTTUB = 0x0436f6e1,
    TWHDVersionTTPV = 0x0436ef7d,

    // Decred
    TWHDVersionDPUB = 0x2fda926,
    TWHDVersionDPRV = 0x2fda4e8,

    // Dogecoin
    TWHDVersionDGUB = 0x02facafd,
    TWHDVersionDGPV = 0x02fac398,
};

/// Determine if the HD Version is public
///
/// \param version HD version
/// \return true if the version is public, false otherwise
TW_EXPORT_PROPERTY
bool TWHDVersionIsPublic(enum TWHDVersion version);

/// Determine if the HD Version is private
///
/// \param version HD version
/// \return true if the version is private, false otherwise
TW_EXPORT_PROPERTY
bool TWHDVersionIsPrivate(enum TWHDVersion version);

TW_EXTERN_C_END
