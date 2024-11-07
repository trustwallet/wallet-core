
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Substrate based chains Address Type
///
/// \see https://github.com/paritytech/substrate/wiki/External-Address-Format-(SS58)#address-type
TW_EXPORT_ENUM(uint8_t)
enum TWSS58AddressType {
    TWSS58AddressTypePolkadot = 0,
    TWSS58AddressTypeKusama = 2,
    TWSS58AddressTypeAstar = 5,
    TWSS58AddressTypeAcala = 10,
    TWSS58AddressTypePolymesh = 12,
    TWSS58AddressTypeParallel = 172,
};

TW_EXTERN_C_END
