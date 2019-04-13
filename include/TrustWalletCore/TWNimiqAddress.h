// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWHRP.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a Nimiq address.
TW_EXPORT_CLASS
struct TWNimiqAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWNimiqAddressEqual(struct TWNimiqAddress *_Nonnull lhs, struct TWNimiqAddress *_Nonnull rhs);

/// Determines if the string is a valid Ripple address.
TW_EXPORT_STATIC_METHOD
bool TWNimiqAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWNimiqAddress *_Nullable TWNimiqAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWNimiqAddress *_Nonnull TWNimiqAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWNimiqAddressDelete(struct TWNimiqAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNimiqAddressDescription(struct TWNimiqAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWNimiqAddressKeyHash(struct TWNimiqAddress *_Nonnull address);

TW_EXTERN_C_END
