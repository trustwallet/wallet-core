#pragma once

#include "TWBase.h"
#include "TWString.h"

#include "TWEOSTransaction.h"

TW_EXTERN_C_BEGIN

/// Represents an EOS transfer action.
TW_EXPORT_CLASS
struct TWEOSPackedTransaction;

TW_EXPORT_STATIC_METHOD
struct TWEOSPackedTransaction *_Nonnull TWEOSPackedTransactionCreate(struct TWEOSTransaction *_Nonnull transaction);

TW_EXPORT_METHOD
TWString *_Nonnull TWEOSPackedTransactionDescription(struct TWEOSPackedTransaction *_Nonnull transaction);

TW_EXPORT_METHOD
void TWEOSPackedTransactionDelete(struct TWEOSPackedTransaction *_Nonnull operation);

TW_EXTERN_C_END