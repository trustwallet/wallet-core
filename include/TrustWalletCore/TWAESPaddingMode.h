// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Padding mode used in AES encryption.
TW_EXPORT_ENUM(uint32_t)
enum TWAESPaddingMode {
    TWAESPaddingModeZero = 0,   // padding value is zero
    TWAESPaddingModePKCS7 = 1,  // padding value is the number of padding bytes; for even size add an extra block
};

TW_EXTERN_C_END
