// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a FIO Account name
TW_EXPORT_CLASS
struct TWFIOAccount;

/// Create a FIO Account
///
/// \param string Account name
/// \note Must be deleted with \TWFIOAccountDelete
/// \return Pointer to a nullable FIO Account
TW_EXPORT_STATIC_METHOD
struct TWFIOAccount *_Nullable TWFIOAccountCreateWithString(TWString *_Nonnull string);

/// Delete a FIO Account
///
/// \param account Pointer to a non-null FIO Account
TW_EXPORT_METHOD
void TWFIOAccountDelete(struct TWFIOAccount *_Nonnull account);

/// Returns the short account string representation.
///
/// \param account Pointer to a non-null FIO Account
/// \return Account non-null string representation
TW_EXPORT_PROPERTY
TWString *_Nonnull TWFIOAccountDescription(struct TWFIOAccount *_Nonnull account);

TW_EXTERN_C_END
