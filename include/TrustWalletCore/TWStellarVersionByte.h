// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_ENUM()
enum class TWStellarVersionByte {
    ACCOUNT_ID = (6 << 3), // G
    SEED = (18 << 3), // S
    PRE_AUTH_TX = (19 << 3), // T
    SHA256_HASH = (23 << 3) // X
};

TW_EXTERN_C_END