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

/// Represents a TON address.
TW_EXPORT_CLASS
struct TWTONAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWTONAddressEqual(struct TWTONAddress *_Nonnull lhs, struct TWTONAddress *_Nonnull rhs);

/// Determines if the string is a valid TON address.
TW_EXPORT_STATIC_METHOD
bool TWTONAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWTONAddress *_Nullable TWTONAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWTONAddress *_Nonnull TWTONAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

/// Delete address object
TW_EXPORT_METHOD
void TWTONAddressDelete(struct TWTONAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWTONAddressDescription(struct TWTONAddress *_Nonnull address);

TW_EXTERN_C_END
