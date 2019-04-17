#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWPrivateKey.h"

#include "TWBravoTransaction.h"

TW_EXTERN_C_BEGIN

/// Represents a Bravo Signer.
TW_EXPORT_CLASS
struct TWBravoSigner;

TW_EXPORT_STATIC_METHOD
struct TWBravoSigner *_Nonnull TWBravoSignerCreate(TWData *_Nonnull chainID);

TW_EXPORT_METHOD
void TWBravoSignerSign(struct TWBravoSigner *_Nonnull signer, struct TWPrivateKey *_Nonnull privateKey, struct TWBravoTransaction *_Nonnull transaction);

TW_EXPORT_METHOD
void TWBravoSignerDelete(struct TWBravoSigner *_Nonnull signer);

TW_EXTERN_C_END