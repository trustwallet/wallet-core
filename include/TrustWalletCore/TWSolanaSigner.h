// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWSolanaProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign Solana transactions.
TW_EXPORT_CLASS
struct TWSolanaSigner;

/// Signs a transaction.
TW_EXPORT_STATIC_METHOD
TW_Solana_Proto_SigningOutput TWSolanaSignerSign(TW_Solana_Proto_SigningInput input);

/// Build a message to be signed.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWSolanaSignerMessage(TW_Solana_Proto_SigningInput data,
                                       TWData* _Nonnull publicKey);

/// Build a transaction to be broadcasted.
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWSolanaSignerTransaction(TW_Solana_Proto_SigningInput data,
                                             TWData* _Nonnull publicKey,
                                             TWData* _Nonnull signature);

TW_EXTERN_C_END
