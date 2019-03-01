// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWBinanceProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign Binance DEX orders.
TW_EXPORT_CLASS
struct TWBinanceSigner;

/// Creates a signer with input data (serialized from BinanceSigningInput)
TW_EXPORT_STATIC_METHOD
struct TWBinanceSigner *_Nonnull TWBinanceSignerCreate(TW_Binance_Proto_SigningInput input);

TW_EXPORT_METHOD
void TWBinanceSignerDelete(struct TWBinanceSigner *_Nonnull signer);

/// Builds a signed transaction.
TW_EXPORT_METHOD
TWData *_Nullable TWBinanceSignerBuild(struct TWBinanceSigner *_Nonnull signer);

TW_EXTERN_C_END
