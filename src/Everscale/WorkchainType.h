#pragma once

#include "TrustWalletCore/TWBase.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_ENUM(int8_t)

namespace TW::Everscale {

enum WorkchainType {
    Masterchain = -1,
    Basechain = 0,
};

} // namespace TW::Everscale

TW_EXTERN_C_END
