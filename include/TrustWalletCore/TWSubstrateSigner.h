// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWSubstrateSigner.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWSubstrateSigner;

/// Builds a message to be signed
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWSubstrateSignerMessage(TWData *_Nonnull data);

/// Builds a transaction to be broadcasted
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWSubstrateSignerTransaction(TWData *_Nonnull data, TWData *_Nonnull publicKey, TWData *_Nonnull signature);

TW_EXTERN_C_END
