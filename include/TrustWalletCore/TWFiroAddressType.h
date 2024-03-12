// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Firo address type.
TW_EXPORT_ENUM(uint32_t)
enum TWFiroAddressType {
    TWFiroAddressTypeDefault = 0, // default
    TWFiroAddressTypeExchange = 1,
};

TW_EXTERN_C_END
