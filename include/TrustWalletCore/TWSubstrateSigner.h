#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWSubstrateProto.h"
#include "TWSubstrateSigner.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWSubstrateSigner;

/// Builds a message to be signed
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWSubstrateSignerMessage(TW_Substrate_Proto_SigningInput data);

/// Builds a transaction to be broadcasted
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWSubstrateSignerTransaction(TW_Substrate_Proto_SigningInput data, TWData *_Nonnull publicKey, TWData *_Nonnull signature);

TW_EXTERN_C_END