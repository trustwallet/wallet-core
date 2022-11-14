// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Preset encryption kind
TW_EXPORT_ENUM(uint32_t)
enum TWStoredKeyEncryption {
    TWAes128Ctr = 0,
    TWAes128Cbc = 1,
    TWAes192Ctr = 2,
    TWAes256Ctr = 3,
};

TW_EXTERN_C_END
