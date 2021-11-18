// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_ENUM(uint32_t)
/// Preset encryption parameter with different security strength, for key store
enum TWStoredKeyEncryptionLevel {
    /// Default, which is one of the below values, determined by the implementation.
    Default = 0,
    /// Minimal sufficient level of encryption strength
    Weak,
    Standard,
};

TW_EXTERN_C_END
