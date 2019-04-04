#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWPrivateKey.h"

#include "TWEOSTransaction.h"
#include "TWEOSKeyType.h"

TW_EXTERN_C_BEGIN

/// Represents a EOS Signer.
TW_EXPORT_CLASS
struct TWEOSSigner;

TW_EXPORT_STATIC_METHOD
struct TWEOSSigner *_Nonnull TWEOSSignerCreate(TWData *_Nonnull chainID);

TW_EXPORT_METHOD
void TWEOSSignerSign(struct TWEOSSigner *_Nonnull signer, struct TWPrivateKey *_Nonnull privateKey, enum TWEOSKeyType type, struct TWEOSTransaction *_Nonnull transaction);

TW_EXPORT_METHOD
void TWEOSSignerDelete(struct TWEOSSigner *_Nonnull signer);

TW_EXTERN_C_END