// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWTransactionDecoder;

/// Decodes a transaction from a binary representation.
///
/// \param coin coin type.
/// \param encodedTx encoded transaction data.
/// \return serialized protobuf message specific for the given coin.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWTransactionDecoderDecode(enum TWCoinType coinType, TWData *_Nonnull encodedTx);

TW_EXTERN_C_END
