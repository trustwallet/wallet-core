#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a Bravo transfer op.
TW_EXPORT_CLASS
struct TWBravoOperation;

TW_EXPORT_STATIC_METHOD
struct TWBravoOperation *_Nullable TWBravoOperationNewTransferOperation(TWString *_Nonnull recipient, TWString *_Nonnull sender, TWString *_Nonnull amount, bool isTestNet, TWString *_Nonnull memo);

TW_EXPORT_METHOD
void TWBravoOperationDelete(struct TWBravoOperation *_Nonnull operation);

TW_EXPORT_STATIC_PROPERTY
size_t TWBravoOperationMaxMemoSize();

TW_EXPORT_STATIC_PROPERTY
size_t TWBravoOperationMaxAccountNameSize();

// No. of decimals used in bravo assets.
// The amount you pass should be (actual_amount * (10^^assetDecimals))
TW_EXPORT_STATIC_PROPERTY
size_t TWBravoOperationAssetDecimals();

TW_EXTERN_C_END