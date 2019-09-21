// Copyright © 2017-2019 Trust Wallet.
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

/// Represents an Algorand address.
TW_EXPORT_CLASS
struct TWAlgorandAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWAlgorandAddressEqual(struct TWAlgorandAddress *_Nonnull lhs, struct TWAlgorandAddress *_Nonnull rhs);

/// Determines if the string is a valid Algorand address.
TW_EXPORT_STATIC_METHOD
bool TWAlgorandAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWAlgorandAddress *_Nullable TWAlgorandAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWAlgorandAddress *_Nonnull TWAlgorandAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWAlgorandAddressDelete(struct TWAlgorandAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWAlgorandAddressDescription(struct TWAlgorandAddress *_Nonnull address);

/// Returns the key hash.
TW_EXPORT_PROPERTY
TWData *_Nonnull TWAlgorandAddressKeyHash(struct TWAlgorandAddress *_Nonnull address);

TW_EXTERN_C_END
