// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWNEARProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign NEAR transactions.
TW_EXPORT_CLASS
struct TWNEARSigner;

/// Signs a transaction.
TW_EXPORT_STATIC_METHOD
TW_NEAR_Proto_SigningOutput TWNEARSignerSign(TW_NEAR_Proto_SigningInput input);

/// Builds a message to be signed
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWNEARSignerMessage(TW_NEAR_Proto_SigningInput data, TWData* _Nonnull publicKey);

/// Builds a transaction to be broadcasted
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWNEARSignerTransaction(TW_NEAR_Proto_SigningInput data, TWData *_Nonnull signature, TWData* _Nonnull publicKey);

TW_EXTERN_C_END