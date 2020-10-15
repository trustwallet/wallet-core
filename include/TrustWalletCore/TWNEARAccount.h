// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a NEAR Account name
TW_EXPORT_CLASS
struct TWNEARAccount;

TW_EXPORT_STATIC_METHOD
struct TWNEARAccount *_Nullable TWNEARAccountCreateWithString(TWString *_Nonnull string);

TW_EXPORT_METHOD
void TWNEARAccountDelete(struct TWNEARAccount *_Nonnull account);

/// Returns the user friendly string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNEARAccountDescription(struct TWNEARAccount *_Nonnull account);

TW_EXTERN_C_END
