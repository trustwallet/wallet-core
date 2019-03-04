// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_ENUM(uint32_t)
enum class TWStellarVersionByte : uint8_t {
    TWStellarVersionByteAccountID = (6 << 3), // G
    TWStellarVersionByteSeed = (18 << 3), // S
    TWStellarVersionBytePreAuthTX = (19 << 3), // T
    TWStellarVersionByteSHA256Hash = (23 << 3) // X
};

TW_EXTERN_C_END
