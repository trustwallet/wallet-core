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

/// Represents an Seele address.
TW_EXPORT_CLASS
struct TWSeeleAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWSeeleAddressEqual(struct TWSeeleAddress *_Nonnull lhs, struct TWSeeleAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWSeeleAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWSeeleAddress *_Nullable TWSeeleAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a key hash.
TW_EXPORT_STATIC_METHOD
struct TWSeeleAddress *_Nullable TWSeeleAddressCreateWithKeyHash(TWData *_Nonnull keyHash);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWSeeleAddress *_Nonnull TWSeeleAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWSeeleAddressDelete(struct TWSeeleAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWSeeleAddressDescription(struct TWSeeleAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWSeeleAddressKeyHash(struct TWSeeleAddress *_Nonnull address);

TW_EXTERN_C_END
