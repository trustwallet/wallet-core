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

/// Represents a Cardano address.
TW_EXPORT_CLASS
struct TWCardanoAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWCardanoAddressEqual(struct TWCardanoAddress *_Nonnull lhs, struct TWCardanoAddress *_Nonnull rhs);

/// Determines if the string is a valid Cardano address.
TW_EXPORT_STATIC_METHOD
bool TWCardanoAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWCardanoAddress *_Nullable TWCardanoAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWCardanoAddress *_Nonnull TWCardanoAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

/// Delete address object
TW_EXPORT_METHOD
void TWCardanoAddressDelete(struct TWCardanoAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWCardanoAddressDescription(struct TWCardanoAddress *_Nonnull address);

TW_EXTERN_C_END
