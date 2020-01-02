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

/// Represents an Decred address.
TW_EXPORT_CLASS
struct TWDecredAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWDecredAddressEqual(struct TWDecredAddress *_Nonnull lhs, struct TWDecredAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWDecredAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWDecredAddress *_Nullable TWDecredAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWDecredAddress *_Nonnull TWDecredAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWDecredAddressDelete(struct TWDecredAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWDecredAddressDescription(struct TWDecredAddress *_Nonnull address);

TW_EXTERN_C_END
