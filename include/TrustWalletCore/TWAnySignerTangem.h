#pragma once

#include <functional>
#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWString.h"

// TANGEM
extern TWData *_Nonnull TWAnySignerSignExternally(TWData* _Nonnull data, enum TWCoinType coin, TWData *_Nonnull publicKey, std::function<const TWData *_Nonnull(const TWData *_Nonnull)> externalSigner);
