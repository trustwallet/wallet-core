#pragma once

#include "TWString.h"

#include "TWEOSAction.h"

TW_EXTERN_C_BEGIN

/// Represents a EOS transaction.
TW_EXPORT_CLASS
struct TWEOSTransaction;

TW_EXPORT_STATIC_METHOD
struct TWEOSTransaction *_Nullable TWEOSTransactionCreate(TWString *_Nonnull referenceBlockId, uint32_t referenceBlockTime);

TW_EXPORT_METHOD
void TWEOSTransactionAddAction(struct TWEOSTransaction *_Nonnull transaction, struct TWEOSAction *_Nonnull action);

TW_EXPORT_METHOD
void TWEOSTransactionAddContextFreeAction(struct TWEOSTransaction *_Nonnull transaction, struct TWEOSAction *_Nonnull action);

TW_EXPORT_PROPERTY
TWString *_Nonnull TWEOSTransactionDescription(struct TWEOSTransaction *_Nonnull transaction);

TW_EXPORT_METHOD
bool TWEOSTransactionIsValid(struct TWEOSTransaction *_Nonnull transaction);

TW_EXPORT_METHOD
void TWEOSTransactionDelete(struct TWEOSTransaction *_Nonnull transaction);

TW_EXTERN_C_END