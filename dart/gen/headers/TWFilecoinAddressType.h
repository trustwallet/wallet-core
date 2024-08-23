// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Filecoin address type.
TW_EXPORT_ENUM(uint32_t)
enum TWFilecoinAddressType {
    TWFilecoinAddressTypeDefault = 0, // default
    TWFilecoinAddressTypeDelegated = 1,
};

TW_EXTERN_C_END
