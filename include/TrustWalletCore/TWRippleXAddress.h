// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"
#include "TWHRP.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a Ripple X-address.
TW_EXPORT_CLASS
struct TWRippleXAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWRippleXAddressEqual(struct TWRippleXAddress *_Nonnull lhs, struct TWRippleXAddress *_Nonnull rhs);

/// Determines if the string is a valid Ripple address.
TW_EXPORT_STATIC_METHOD
bool TWRippleXAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWRippleXAddress *_Nullable TWRippleXAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key and destination tag.
TW_EXPORT_STATIC_METHOD
struct TWRippleXAddress *_Nonnull TWRippleXAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint32_t tag);

TW_EXPORT_METHOD
void TWRippleXAddressDelete(struct TWRippleXAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWRippleXAddressDescription(struct TWRippleXAddress *_Nonnull address);

/// Returns the destination tag.
TW_EXPORT_PROPERTY
uint32_t TWRippleXAddressTag(struct TWRippleXAddress *_Nonnull address);

TW_EXTERN_C_END
