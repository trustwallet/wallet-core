// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWStellarProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign Stellar transactions.
TW_EXPORT_CLASS
struct TWStellarSigner;

/// Signs a transaction.
TW_EXPORT_STATIC_METHOD
TW_Stellar_Proto_SigningOutput TWStellarSignerSign(TW_Stellar_Proto_SigningInput input);

/// Builds a message to be signed
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWStellarSignerMessage(TW_Stellar_Proto_SigningInput data);

/// Builds a transaction to be broadcasted
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWStellarSignerTransaction(TW_Stellar_Proto_SigningInput data, TWData *_Nonnull signature);

TW_EXTERN_C_END
