// Copyright © 2019 Mart Roosmaa.
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

/// Represents a Nano address.
TW_EXPORT_CLASS
struct TWNanoAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWNanoAddressEqual(struct TWNanoAddress *_Nonnull lhs, struct TWNanoAddress *_Nonnull rhs);

/// Determines if the string is a valid Nano address.
TW_EXPORT_STATIC_METHOD
bool TWNanoAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWNanoAddress *_Nullable TWNanoAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWNanoAddress *_Nonnull TWNanoAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWNanoAddressDelete(struct TWNanoAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNanoAddressDescription(struct TWNanoAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWNanoAddressKeyHash(struct TWNanoAddress *_Nonnull address);

TW_EXTERN_C_END
