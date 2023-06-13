// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWPolkadotProto.h"
#include "TWPolkadotSigner.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWPolkadotSigner;

/// Builds a message to be signed
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWPolkadotSignerMessage(TW_Polkadot_Proto_SigningInput data);

/// Builds a transaction to be broadcasted
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWPolkadotSignerTransaction(TW_Polkadot_Proto_SigningInput data, TWData *_Nonnull publicKey, TWData *_Nonnull signature);

TW_EXTERN_C_END