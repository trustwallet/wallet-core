#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a Steem transfer op.
TW_EXPORT_CLASS
struct TWSteemOperation;

TW_EXPORT_STATIC_METHOD
struct TWSteemOperation *_Nullable TWSteemOperationNewTransferOperation(TWString *_Nonnull recipient, TWString *_Nonnull sender, uint64_t amount, bool isTestNet, TWString *_Nonnull memo);

TW_EXPORT_METHOD
void TWSteemOperationDelete(struct TWSteemOperation *_Nonnull operation);

TW_EXPORT_STATIC_PROPERTY
size_t TWSteemOperationMaxMemoSize();

TW_EXPORT_STATIC_PROPERTY
size_t TWSteemOperationMaxAccountNameSize();

// No. of decimals used in steem assets.
// The amount you pass should be (actual_amount * (10^^assetDecimals))
TW_EXPORT_STATIC_PROPERTY
size_t TWSteemOperationAssetDecimals();

TW_EXTERN_C_END