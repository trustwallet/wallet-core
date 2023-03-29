// Copyright © 2017-2023 Trust Wallet.
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

/// Create a NEAR Account
///
/// \param string Account name
/// \note Account should be deleted by calling \TWNEARAccountDelete
/// \return Pointer to a nullable NEAR Account.
TW_EXPORT_STATIC_METHOD
struct TWNEARAccount *_Nullable TWNEARAccountCreateWithString(TWString *_Nonnull string);

/// Delete the given Near Account
///
/// \param account Pointer to a non-null NEAR Account
TW_EXPORT_METHOD
void TWNEARAccountDelete(struct TWNEARAccount *_Nonnull account);

/// Returns the user friendly string representation.
///
/// \param account Pointer to a non-null NEAR Account
/// \return Non-null string account description
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNEARAccountDescription(struct TWNEARAccount *_Nonnull account);

TW_EXTERN_C_END
