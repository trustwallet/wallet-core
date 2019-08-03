#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_ENUM(uint32_t)
enum TWBitcoinSigHashType {
    TWBitcoinSigHashTypeAll = 0x01,
    TWBitcoinSigHashTypeNone = 0x02,
    TWBitcoinSigHashTypeSingle = 0x03,
    TWBitcoinSigHashTypeFork = 0x40,
    TWBitcoinSigHashTypeAnyoneCanPay = 0x80
};

extern bool TWBitcoinSigHashTypeIsSingle(uint32_t type);
extern bool TWBitcoinSigHashTypeIsNone(uint32_t type);

TW_EXTERN_C_END
