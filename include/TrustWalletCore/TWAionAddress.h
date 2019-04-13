// Copyright © 2017-2019 Trust.
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

/// Represents an Aion address.
TW_EXPORT_CLASS
struct TWAionAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWAionAddressEqual(struct TWAionAddress *_Nonnull lhs, struct TWAionAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWAionAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWAionAddress *_Nullable TWAionAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWAionAddress *_Nonnull TWAionAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWAionAddressDelete(struct TWAionAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWAionAddressDescription(struct TWAionAddress *_Nonnull address);

TW_EXTERN_C_END
