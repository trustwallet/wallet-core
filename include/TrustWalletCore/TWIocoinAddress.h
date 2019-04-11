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

/// Represents a legacy Iocoin address.
TW_EXPORT_CLASS
struct TWIocoinAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWIocoinAddressEqual(struct TWIocoinAddress *_Nonnull lhs, struct TWIocoinAddress *_Nonnull rhs);

/// Determines if the string is a valid Iocoin address.
TW_EXPORT_STATIC_METHOD
bool TWIocoinAddressIsValidString(TWString *_Nonnull string);

/// Create an address from a base58 sring representaion.
TW_EXPORT_STATIC_METHOD
struct TWIocoinAddress *_Nullable TWIocoinAddressCreateWithString(TWString *_Nonnull string);

/// Create an address from a public key and a prefix byte.
TW_EXPORT_STATIC_METHOD
struct TWIocoinAddress *_Nonnull TWIocoinAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint8_t prefix);

TW_EXPORT_METHOD
void TWIocoinAddressDelete(struct TWIocoinAddress *_Nonnull address);

/// Returns the address base58 string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWIocoinAddressDescription(struct TWIocoinAddress *_Nonnull address);

TW_EXTERN_C_END
