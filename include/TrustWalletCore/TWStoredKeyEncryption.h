// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Preset encryption kind
TW_EXPORT_ENUM(uint32_t)
enum TWStoredKeyEncryption {
    TWStoredKeyEncryptionAes128Ctr = 0,
    TWStoredKeyEncryptionAes128Cbc = 1,
    TWStoredKeyEncryptionAes192Ctr = 2,
    TWStoredKeyEncryptionAes256Ctr = 3,
};

TW_EXTERN_C_END
