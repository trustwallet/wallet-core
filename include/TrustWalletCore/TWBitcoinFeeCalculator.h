// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWData.h"
#include <optional>

TW_EXTERN_C_BEGIN

std::optional<uint64_t> TWBitcoinCalculateFee(TWData *_Nonnull data, uint64_t satVb);

TW_EXTERN_C_END
