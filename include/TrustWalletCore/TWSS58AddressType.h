
// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
};

TW_EXTERN_C_END
