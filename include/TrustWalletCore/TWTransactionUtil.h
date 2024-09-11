// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWTransactionUtil;

/// Calculate the TX hash of a transaction.
///
/// \param coin coin type.
/// \param encodedTx encoded transaction data.
/// \return The TX hash of a transaction, If the input is invalid or the chain is unsupported, null is returned.
TW_EXPORT_STATIC_METHOD
TWString* _Nullable TWTransactionUtilCalcTxHash(enum TWCoinType coinType, TWString* _Nonnull encodedTx);

TW_EXTERN_C_END
