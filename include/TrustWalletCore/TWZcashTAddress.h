// Copyright © 2017-2020 Trust Wallet.
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

static const size_t TWZcashTAddressSize = 22;

/// Represents a transparent Zcash address.
TW_EXPORT_CLASS
struct TWZcashTAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWZcashTAddressEqual(struct TWZcashTAddress *_Nonnull lhs, struct TWZcashTAddress *_Nonnull rhs);

/// Determines if the data is a valid Bitcoin address.
TW_EXPORT_STATIC_METHOD
bool TWZcashTAddressIsValid(TWData *_Nonnull data);

/// Determines if the string is a valid Bitcoin address.
TW_EXPORT_STATIC_METHOD
bool TWZcashTAddressIsValidString(TWString *_Nonnull string);

/// Initializes an address from a base58 sring representaion.
TW_EXPORT_STATIC_METHOD
struct TWZcashTAddress *_Nullable TWZcashTAddressCreateWithString(TWString *_Nonnull string);

/// Initializes an address from raw data.
TW_EXPORT_STATIC_METHOD
struct TWZcashTAddress *_Nullable TWZcashTAddressCreateWithData(TWData *_Nonnull data);

/// Initializes an address from a public key and a prefix byte.
TW_EXPORT_STATIC_METHOD
struct TWZcashTAddress *_Nullable TWZcashTAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix);

TW_EXPORT_METHOD
void TWZcashTAddressDelete(struct TWZcashTAddress *_Nonnull address);

/// Returns the address base58 string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWZcashTAddressDescription(struct TWZcashTAddress *_Nonnull address);

TW_EXTERN_C_END
