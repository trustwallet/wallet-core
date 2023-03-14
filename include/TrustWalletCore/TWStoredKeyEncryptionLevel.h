// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
