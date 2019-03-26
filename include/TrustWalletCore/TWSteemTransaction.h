// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWString.h"

#include "TWSteemOperation.h"

TW_EXTERN_C_BEGIN

/// Represents a Steem transaction.
TW_EXPORT_CLASS
struct TWSteemTransaction;

TW_EXPORT_STATIC_METHOD
struct TWSteemTransaction *_Nullable TWSteemTransactionCreate(TWString *_Nonnull referenceBlockId, uint32_t referenceBlockTime);

TW_EXPORT_METHOD
void TWSteemTransactionAddOperation(struct TWSteemTransaction *_Nonnull transaction, struct TWSteemOperation *_Nonnull operation);

TW_EXPORT_METHOD
bool TWSteemTransactionIsValid(struct TWSteemTransaction *_Nonnull transaction);

TW_EXPORT_METHOD
void TWSteemTransactionDelete(struct TWSteemTransaction *_Nonnull transaction);

TW_EXTERN_C_END