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

/// Represents a Ripple address.
TW_EXPORT_CLASS
struct TWRippleAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWRippleAddressEqual(struct TWRippleAddress *_Nonnull lhs, struct TWRippleAddress *_Nonnull rhs);

/// Determines if the string is a valid Ripple address.
TW_EXPORT_STATIC_METHOD
bool TWRippleAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWRippleAddress *_Nullable TWRippleAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a raw data representaion.
TW_EXPORT_STATIC_METHOD
struct TWRippleAddress *_Nullable TWRippleAddressCreateWithData(TWData *_Nonnull data);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWRippleAddress *_Nonnull TWRippleAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWRippleAddressDelete(struct TWRippleAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWRippleAddressDescription(struct TWRippleAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWRippleAddressKeyHash(struct TWRippleAddress *_Nonnull address);

TW_EXTERN_C_END
