// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Helper class to decode Avalanche binary data.
TW_EXPORT_CLASS
struct TWAvalancheDecoder;

/// Encode function to Eth ABI binary
TW_EXPORT_STATIC_METHOD
TWString* _Nullable TWAvalancheDecoderDecodeUTXO(TWData* _Nonnull data);

TW_EXTERN_C_END
