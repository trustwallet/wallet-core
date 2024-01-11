// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Stellar memo type.
TW_EXPORT_ENUM(uint32_t)
enum TWStellarMemoType {
    TWStellarMemoTypeNone = 0,
    TWStellarMemoTypeText = 1,
    TWStellarMemoTypeId = 2,
    TWStellarMemoTypeHash = 3,
    TWStellarMemoTypeReturn = 4,
};

TW_EXTERN_C_END
