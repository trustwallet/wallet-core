// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"
#include "TWIconProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign Icon transactions.
TW_EXPORT_CLASS
struct TWIconSigner;

/// Signs a transaction.
TW_EXPORT_STATIC_METHOD
TW_Icon_Proto_SigningOutput TWIconSignerSign(TW_Icon_Proto_SigningInput input);

TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWIconSignerMessage(TW_Icon_Proto_SigningInput data);

TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWIconSignerTransaction(TW_Icon_Proto_SigningInput data, TWData *_Nonnull signature);

TW_EXTERN_C_END
