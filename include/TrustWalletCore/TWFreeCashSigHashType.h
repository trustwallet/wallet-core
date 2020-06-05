// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_ENUM(uint32_t)
enum TWFreeCashSigHashType {
    TWFreeCashSigHashTypeAll = 0x01,
    TWFreeCashSigHashTypeNone = 0x02,
    TWFreeCashSigHashTypeSingle = 0x03,
    TWFreeCashSigHashTypeFork = 0x40,
    TWFreeCashSigHashTypeAnyoneCanPay = 0x80
};

TW_EXPORT_METHOD
bool TWFreeCashSigHashTypeIsSingle(enum TWFreeCashSigHashType type);

TW_EXPORT_METHOD
bool TWFreeCashSigHashTypeIsNone(enum TWFreeCashSigHashType type);

TW_EXTERN_C_END
