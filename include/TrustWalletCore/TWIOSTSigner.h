// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWIOSTProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign IOST transactions.
TW_EXPORT_CLASS
struct TWIOSTSigner;

/// Signs a transaction.
TW_EXPORT_STATIC_METHOD
TW_IOST_Proto_SigningOutput TWIOSTSignerSign(TW_IOST_Proto_SigningInput input);

/// Builds a message to be signed
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWIOSTSignerMessage(TW_IOST_Proto_SigningInput input, TWData *_Nonnull pubkey, uint8_t algorithm);

TW_EXTERN_C_END