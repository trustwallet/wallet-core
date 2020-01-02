// Copyright © 2017-2020 Trust Wallet.
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

/// Represents a Tron address.
TW_EXPORT_CLASS
struct TWTronAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWTronAddressEqual(struct TWTronAddress *_Nonnull lhs, struct TWTronAddress *_Nonnull rhs);

/// Determines if the string is a valid Tron address.
TW_EXPORT_STATIC_METHOD
bool TWTronAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWTronAddress *_Nullable TWTronAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWTronAddress *_Nonnull TWTronAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWTronAddressDelete(struct TWTronAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWTronAddressDescription(struct TWTronAddress *_Nonnull address);

TW_EXTERN_C_END
