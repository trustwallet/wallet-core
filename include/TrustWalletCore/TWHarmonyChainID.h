// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Chain identifier for Harmony blockchain.
TW_EXPORT_ENUM(uint32_t)
enum TWHarmonyChainID {
    TWHarmonyChainIDMainNet = 0x1,
};

TW_EXTERN_C_END
