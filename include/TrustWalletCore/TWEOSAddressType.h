#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_ENUM(uint32_t)
enum TWEOSAddressType {
    TWEOSAddressTypeLegacy = 0,
    TWEOSAddressTypeModernK1 = 1,
};

TW_EXTERN_C_END