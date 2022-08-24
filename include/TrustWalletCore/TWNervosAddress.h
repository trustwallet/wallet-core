// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a Nervos address.
TW_EXPORT_CLASS
struct TWNervosAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWNervosAddressEqual(struct TWNervosAddress *_Nonnull lhs, struct TWNervosAddress *_Nonnull rhs);

/// Determines if the string is a valid Nervos address.
TW_EXPORT_STATIC_METHOD
bool TWNervosAddressIsValidString(TWString *_Nonnull string);

/// Initializes an address from a sring representaion.
TW_EXPORT_STATIC_METHOD
struct TWNervosAddress *_Nullable TWNervosAddressCreateWithString(TWString *_Nonnull string);

TW_EXPORT_METHOD
void TWNervosAddressDelete(struct TWNervosAddress *_Nonnull address);

/// Returns the address base58 string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNervosAddressDescription(struct TWNervosAddress *_Nonnull address);

/// Returns the keyhash data.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWNervosAddressCodeHash(struct TWNervosAddress *_Nonnull address);

/// Returns the address prefix.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNervosAddressHashType(struct TWNervosAddress *_Nonnull address);

/// Returns the keyhash data.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWNervosAddressArgs(struct TWNervosAddress *_Nonnull address);

TW_EXTERN_C_END
