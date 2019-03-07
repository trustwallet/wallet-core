// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"
#include "TWIconAddressType.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents an Icon address.
TW_EXPORT_CLASS
struct TWIconAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWIconAddressEqual(struct TWIconAddress *_Nonnull lhs, struct TWIconAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWIconAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWIconAddress *_Nullable TWIconAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a key hash.
TW_EXPORT_STATIC_METHOD
struct TWIconAddress *_Nullable TWIconAddressCreateWithKeyHash(TWData *_Nonnull keyHash, enum TWIconAddressType type);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWIconAddress *_Nonnull TWIconAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, enum TWIconAddressType type);

TW_EXPORT_METHOD
void TWIconAddressDelete(struct TWIconAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWIconAddressDescription(struct TWIconAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWIconAddressKeyHash(struct TWIconAddress *_Nonnull address);

TW_EXTERN_C_END
