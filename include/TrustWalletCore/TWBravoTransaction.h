#pragma once

#include "TWString.h"

#include "TWBravoOperation.h"

TW_EXTERN_C_BEGIN

/// Represents a Bravo transaction.
TW_EXPORT_CLASS
struct TWBravoTransaction;

TW_EXPORT_STATIC_METHOD
struct TWBravoTransaction *_Nullable TWBravoTransactionCreate(TWString *_Nonnull referenceBlockId, uint32_t referenceBlockTime);

TW_EXPORT_METHOD
void TWBravoTransactionAddOperation(struct TWBravoTransaction *_Nonnull transaction, struct TWBravoOperation *_Nonnull operation);

TW_EXPORT_PROPERTY
TWString *_Nonnull TWBravoTransactionDescription(struct TWBravoTransaction *_Nonnull transaction);

TW_EXPORT_METHOD
bool TWBravoTransactionIsValid(struct TWBravoTransaction *_Nonnull transaction);

TW_EXPORT_METHOD
void TWBravoTransactionDelete(struct TWBravoTransaction *_Nonnull transaction);

TW_EXTERN_C_END