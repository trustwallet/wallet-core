// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Icon address types
TW_EXPORT_ENUM()
enum TWIconAddressType {
    TWIconAddressTypeAddress  /* "hx" */,
    TWIconAddressTypeContract /* "cx" */,
};

TW_EXTERN_C_END
