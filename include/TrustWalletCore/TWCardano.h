// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWCardanoProto.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWCardano;

// The minimum ADA amount needed for a UTXO.  See https://docs.cardano.org/native-tokens/minimum-ada-value-requirement
TW_EXPORT_STATIC_METHOD
uint64_t TWCardanoMinAdaAmount(TW_Cardano_Proto_TokenBundle tokenBundle) TW_VISIBILITY_DEFAULT;

TW_EXTERN_C_END
