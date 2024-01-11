// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Preset encryption parameter with different security strength, for key store
TW_EXPORT_ENUM(uint32_t)
enum TWStoredKeyEncryptionLevel {
    /// Default, which is one of the below values, determined by the implementation.
    TWStoredKeyEncryptionLevelDefault = 0,
    /// Minimal sufficient level of encryption strength (scrypt 4096)
    TWStoredKeyEncryptionLevelMinimal = 1,
    /// Weak encryption strength (scrypt 16k)
    TWStoredKeyEncryptionLevelWeak = 2,
    /// Standard level of encryption strength (scrypt 262k)
    TWStoredKeyEncryptionLevelStandard = 3,
};

TW_EXTERN_C_END
