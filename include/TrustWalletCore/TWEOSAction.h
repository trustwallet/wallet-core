#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents an EOS transfer action.
TW_EXPORT_CLASS
struct TWEOSAction;

TW_EXPORT_STATIC_METHOD
struct TWEOSAction *_Nullable TWEOSActionNewTransferAction(TWString *_Nonnull currency, TWString *_Nonnull actor, TWString *_Nonnull recipient, TWString *_Nonnull sender, TWString *_Nonnull assetString, TWString *_Nonnull memo);

TW_EXPORT_METHOD
void TWEOSActionDelete(struct TWEOSAction *_Nonnull operation);

TW_EXTERN_C_END