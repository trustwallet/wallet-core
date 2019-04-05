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

struct TWPublicKey;

/// Represents a NEO address.
TW_EXPORT_CLASS
struct TWNEOAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWNEOAddressEqual(struct TWNEOAddress *_Nonnull lhs, struct TWNEOAddress *_Nonnull rhs);

/// Determines if the string is a valid NEO address.
TW_EXPORT_STATIC_METHOD
bool TWNEOAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWNEOAddress *_Nullable TWNEOAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWNEOAddress *_Nonnull TWNEOAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWNEOAddressDelete(struct TWNEOAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNEOAddressDescription(struct TWNEOAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWNEOAddressKeyHash(struct TWNEOAddress *_Nonnull address);

TW_EXTERN_C_END
