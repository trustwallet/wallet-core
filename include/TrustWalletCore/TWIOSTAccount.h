// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents an IOST Account.
TW_EXPORT_CLASS
struct TWIOSTAccount;

/// Compares two Accountes for equality.
TW_EXPORT_STATIC_METHOD
bool TWIOSTAccountEqual(struct TWIOSTAccount *_Nonnull lhs, struct TWIOSTAccount *_Nonnull rhs);

/// Determines if the string is a valid Account.
TW_EXPORT_STATIC_METHOD
bool TWIOSTAccountIsValidString(TWString *_Nonnull string);

/// Creates an Account from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWIOSTAccount *_Nullable TWIOSTAccountCreateWithString(TWString *_Nonnull string);

TW_EXPORT_METHOD
void TWIOSTAccountDelete(struct TWIOSTAccount *_Nonnull Account);

/// Returns the Account string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWIOSTAccountDescription(struct TWIOSTAccount *_Nonnull Account);

TW_EXTERN_C_END
