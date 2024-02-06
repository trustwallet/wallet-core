// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWDataVector.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWSolanaTransaction;

/// Decode Solana transaction, update the recent blockhash and re-sign the transaction.
///
/// # Warning
///
/// This is a temporary solution. It will be removed when `Solana.proto` supports
/// direct transaction signing.
///
/// \param encoded_tx base64 encoded Solana transaction.
/// \param recent_blockhash base58 encoded recent blockhash.
/// \param private_keys list of private keys that should be used to re-sign the transaction.
/// \return serialized `Solana::Proto::SigningOutput`.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWSolanaTransactionUpdateBlockhashAndSign(const TWString *_Nonnull encodedTx,
                                                           TWString *_Nonnull recentBlockhash,
                                                           const struct TWDataVector *_Nonnull privateKeys);

TW_EXTERN_C_END
