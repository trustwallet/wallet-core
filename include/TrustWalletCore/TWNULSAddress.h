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

/// Represents a NULS address.
TW_EXPORT_CLASS
struct TWNULSAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWNULSAddressEqual(struct TWNULSAddress *_Nonnull lhs, struct TWNULSAddress *_Nonnull rhs);

/// Determines if the string is a valid NULS address.
TW_EXPORT_STATIC_METHOD
bool TWNULSAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWNULSAddress *_Nullable TWNULSAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWNULSAddress *_Nonnull TWNULSAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

/// Delete an address instance.
TW_EXPORT_METHOD
void TWNULSAddressDelete(struct TWNULSAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNULSAddressDescription(struct TWNULSAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWNULSAddressKeyHash(struct TWNULSAddress *_Nonnull address);

TW_EXTERN_C_END