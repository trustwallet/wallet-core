#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_ENUM(uint32_t)
enum TWEOSKeyType {
    TWEOSKeyTypeLegacy = 0,
    TWEOSKeyTypeModernK1 = 1,
    TWEOSKeyTypeModernR1 = 2,
};

TW_EXTERN_C_END